from datetime import datetime, timedelta, timezone

from app.config import DecisionConfig
from app.models import FluxSample, TrackPoint
from app.services.decision_service import DecisionService


def _track(n=5, start=None):
    t0 = start or datetime.now(timezone.utc)
    return [
        TrackPoint(
            t=t0 + timedelta(seconds=60 * i),
            lat=0.0,
            lon=0.0,
            alt_km=800.0,
            tle_epoch=t0,
            orbit_quality=1.0,
        )
        for i in range(n)
    ]


def test_decision_thresholds():
    cfg = DecisionConfig(r_off=2.0, r_on=1.0, hold_time_sec=0, min_off_time_sec=0, min_on_time_sec=0)
    svc = DecisionService(cfg)
    t0 = datetime.now(timezone.utc)
    track = _track(4, start=t0)
    flux_series = [
        FluxSample(t=track[0].t, channel="Je>100keV", value=1e2, percentile="mean"),
        FluxSample(t=track[1].t, channel="Je>100keV", value=1e4, percentile="mean"),
        FluxSample(t=track[2].t, channel="Je>100keV", value=1e2, percentile="mean"),
        FluxSample(t=track[3].t, channel="Je>100keV", value=1e2, percentile="mean"),
    ]
    risks = svc.compute_risk_series(flux_series)
    windows = svc.decide_windows(track, risks)
    assert any(w.mode == "OBS_OFF" for w in windows)
    assert any(w.mode == "OBS_ON" for w in windows)

