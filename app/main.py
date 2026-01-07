from __future__ import annotations

import asyncio
import logging
from datetime import datetime, timedelta, timezone
from typing import Dict, List, Optional

from dateutil import parser as dateparser
from fastapi import Body, FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

from app.config import AppConfig, ConfigStore, load_config
from app.models import FluxSample, OrbitState, TrackPoint, TLESet, Window
from app.services.decision_service import DecisionService
from app.services.flux_service import FluxService
from app.services.orbit_service import OrbitService
from app.services.tle_service import TLEQuality, TLERepository, TLEService

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")
logger = logging.getLogger("mass-cube")


app = FastAPI(title="MASS-Cube Backend", version="0.1.0")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

config_store: ConfigStore = load_config()
tle_repo = TLERepository()
tle_quality = TLEQuality(config_store.config.tle_quality)
tle_service = TLEService(tle_repo, tle_quality)
orbit_service = OrbitService(config_store.config.orbits)
flux_service = FluxService(config_store.config.flux)
decision_service = DecisionService(config_store.config.decision)
_refresh_task: Optional[asyncio.Task] = None


def rebuild_services() -> None:
    global tle_quality, tle_service, orbit_service, flux_service, decision_service
    cfg = config_store.config
    tle_quality = TLEQuality(cfg.tle_quality)
    tle_service = TLEService(tle_repo, tle_quality)
    orbit_service = OrbitService(cfg.orbits)
    flux_service = FluxService(cfg.flux)
    decision_service = DecisionService(cfg.decision)
    logger.info("Services rebuilt with config version %s", config_store.version)


async def tle_refresh_loop() -> None:
    while True:
        try:
            await tle_service.fetch_and_store()
        except Exception as exc:  # pragma: no cover - best-effort background loop
            logger.error("TLE refresh failed: %s", exc)
        await asyncio.sleep(config_store.config.orbits.refresh_rate_sec)


@app.on_event("startup")
async def startup_event() -> None:
    global _refresh_task
    logger.info("Starting up with config version %s", config_store.version)
    _refresh_task = asyncio.create_task(tle_refresh_loop())


@app.on_event("shutdown")
async def shutdown_event() -> None:
    if _refresh_task:
        _refresh_task.cancel()


@app.get("/healthz")
async def healthz() -> Dict[str, str]:
    return {"status": "ok", "version": app.version}


@app.get("/config")
async def get_config() -> Dict:
    return {
        "version": config_store.version,
        "config": config_store.config.model_dump(mode="json"),
        "history_size": len(config_store.history),
    }


@app.put("/config")
async def update_config(payload: Dict = Body(...)) -> Dict:
    try:
        cfg = config_store.update(payload, source="api")
    except Exception as exc:
        raise HTTPException(status_code=400, detail=str(exc))
    rebuild_services()
    return {"version": config_store.version, "config": cfg.model_dump(mode="json")}


def _parse_time(param: Optional[str]) -> datetime:
    if not param:
        return datetime.now(timezone.utc)
    try:
        return dateparser.isoparse(param).astimezone(timezone.utc)
    except Exception as exc:
        raise HTTPException(status_code=400, detail=f"Invalid time format: {exc}")


async def _get_best_tle(at_time: datetime) -> Optional[TLESet]:
    tle = tle_service.select_for_time(at_time, config_store.config.orbits.history_days)
    if tle:
        return tle
    return await tle_service.fetch_and_store()


@app.get("/sat/state", response_model=OrbitState)
async def sat_state(time: Optional[str] = None) -> OrbitState:
    t = _parse_time(time)
    tle = await _get_best_tle(t)
    if not tle:
        raise HTTPException(status_code=503, detail="No TLE available")
    state = orbit_service.propagate(tle, t)
    return state


@app.get("/sat/track", response_model=List[TrackPoint])
async def sat_track(start: Optional[str] = None, end: Optional[str] = None, step: Optional[int] = None) -> List[TrackPoint]:
    if start is None and end is None:
        now = datetime.now(timezone.utc)
        half = config_store.config.ui.track_length_sec / 2
        start_dt = now - timedelta(seconds=half)
        end_dt = now + timedelta(seconds=half)
        tle_ref = now
    else:
        start_dt = _parse_time(start)
        end_dt = _parse_time(end) if end else start_dt + timedelta(seconds=config_store.config.orbits.prediction_horizon_sec)
        tle_ref = start_dt
    step_sec = step or config_store.config.orbits.track_step_sec
    if end_dt <= start_dt:
        raise HTTPException(status_code=400, detail="end must be after start")
    tle = await _get_best_tle(tle_ref)
    if not tle:
        raise HTTPException(status_code=503, detail="No TLE available")
    return orbit_service.track(tle, start_dt, end_dt, step_sec)


@app.get("/env/flux/track", response_model=List[FluxSample])
async def flux_track(
    start: Optional[str] = None,
    end: Optional[str] = None,
    step: Optional[int] = None,
    percentile: Optional[str] = None,
) -> List[FluxSample]:
    step_sec = step or config_store.config.orbits.track_step_sec
    track = await sat_track(start=start, end=end, step=step_sec)
    pct = percentile or config_store.config.flux.percentile_default
    return flux_service.compute_series(track, pct)


@app.get("/decision/windows", response_model=List[Window])
async def decision_windows(
    start: Optional[str] = None, end: Optional[str] = None, step: Optional[int] = None, percentile: Optional[str] = None
) -> List[Window]:
    track = await sat_track(start=start, end=end, step=step)
    pct = percentile or config_store.config.flux.percentile_default
    flux_series = flux_service.compute_series(track, pct)
    risks = decision_service.compute_risk_series(flux_series)
    return decision_service.decide_windows(track, risks)
