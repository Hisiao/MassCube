from __future__ import annotations

import json
import sqlite3
from contextlib import closing
from dataclasses import dataclass
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import List, Optional, Tuple

import httpx
from dateutil import parser as dateparser
from sgp4.api import Satrec, WGS72
from sgp4.exporter import export_tle

from app.config import TLEQualityConfig
from app.models import TLESet

DB_PATH = Path("data/tle_history.db")


@dataclass
class TLESourceConfig:
    primary_url: str = "http://8.141.94.91/tle/ty47.txt"
    fallback_url: str = "https://celestrak.org/NORAD/elements/gp.php"
    primary_user: Optional[str] = None
    primary_password: Optional[str] = None


class TLERepository:
    def __init__(self, db_path: Path = DB_PATH):
        self.db_path = db_path
        self.db_path.parent.mkdir(parents=True, exist_ok=True)
        self._init_db()

    def _init_db(self) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute(
                """
                CREATE TABLE IF NOT EXISTS tles(
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    norad_id INTEGER,
                    source TEXT,
                    fetched_at TEXT,
                    epoch TEXT,
                    line1 TEXT,
                    line2 TEXT,
                    score REAL,
                    warnings TEXT
                );
                """
            )
            conn.execute("CREATE INDEX IF NOT EXISTS idx_tles_norad ON tles(norad_id)")

    def save(self, tle: TLESet) -> None:
        with sqlite3.connect(self.db_path) as conn:
            conn.execute(
                """
                INSERT INTO tles (norad_id, source, fetched_at, epoch, line1, line2, score, warnings)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (
                    tle.norad_id,
                    tle.source,
                    tle.fetched_at.isoformat(),
                    tle.epoch.isoformat(),
                    tle.line1,
                    tle.line2,
                    tle.score,
                    json.dumps(tle.warnings),
                ),
            )

    def list_recent(self, norad_id: int, days: int) -> List[TLESet]:
        since = datetime.now(timezone.utc) - timedelta(days=days)
        with sqlite3.connect(self.db_path) as conn:
            conn.row_factory = sqlite3.Row
            rows = conn.execute(
                "SELECT * FROM tles WHERE norad_id = ? AND fetched_at >= ? ORDER BY fetched_at DESC",
                (norad_id, since.isoformat()),
            ).fetchall()
        return [
            TLESet(
                norad_id=row["norad_id"],
                source=row["source"],
                fetched_at=dateparser.isoparse(row["fetched_at"]),
                epoch=dateparser.isoparse(row["epoch"]),
                line1=row["line1"],
                line2=row["line2"],
                score=row["score"],
                warnings=json.loads(row["warnings"]),
            )
            for row in rows
        ]

    def latest(self, norad_id: int) -> Optional[TLESet]:
        with sqlite3.connect(self.db_path) as conn:
            conn.row_factory = sqlite3.Row
            row = conn.execute(
                "SELECT * FROM tles WHERE norad_id = ? ORDER BY fetched_at DESC LIMIT 1", (norad_id,)
            ).fetchone()
        if not row:
            return None
        return TLESet(
            norad_id=row["norad_id"],
            source=row["source"],
            fetched_at=dateparser.isoparse(row["fetched_at"]),
            epoch=dateparser.isoparse(row["epoch"]),
            line1=row["line1"],
            line2=row["line2"],
            score=row["score"],
            warnings=json.loads(row["warnings"]),
        )


class TLEQuality:
    def __init__(self, cfg: TLEQualityConfig):
        self.cfg = cfg

    def assess(self, tle: TLESet, previous: Optional[TLESet]) -> TLESet:
        score = 1.0
        warnings: List[str] = []

        now = datetime.now(timezone.utc)
        age_sec = (now - tle.epoch).total_seconds()
        if age_sec > self.cfg.freshness_max_sec:
            freshness_penalty = min(1.0, age_sec / self.cfg.freshness_max_sec - 1)
            score -= freshness_penalty * self.cfg.score_weights.get("freshness", 0.3)
            warnings.append(f"TLE stale by {int(age_sec)}s")

        if previous:
            deltas = self._element_changes(previous, tle)
            for key, limit in self.cfg.element_change_threshold.items():
                if abs(deltas.get(key, 0.0)) > limit:
                    score -= self.cfg.score_weights.get("stability", 0.3) * 0.5
                    warnings.append(f"Element jump {key} {deltas[key]:.4f}")

            bias = self._propagation_bias(previous, tle)
            if bias > self.cfg.propagation_tolerance_km:
                score -= self.cfg.score_weights.get("propagation", 0.3) * min(1.0, bias / self.cfg.propagation_tolerance_km)
                warnings.append(f"Propagation bias {bias:.1f} km")

        tle.score = max(score, 0.0)
        tle.warnings = warnings
        return tle

    def _element_changes(self, old: TLESet, new: TLESet) -> dict:
        sat_old = Satrec.twoline2rv(old.line1, old.line2, WGS72)
        sat_new = Satrec.twoline2rv(new.line1, new.line2, WGS72)
        return {
            "mean_motion": sat_new.no_kozai - sat_old.no_kozai,
            "inclination_deg": sat_new.inclo - sat_old.inclo,
            "raan_deg": sat_new.nodeo - sat_old.nodeo,
            "eccentricity": sat_new.ecco - sat_old.ecco,
        }

    def _propagation_bias(self, previous: TLESet, new: TLESet) -> float:
        sat_prev = Satrec.twoline2rv(previous.line1, previous.line2, WGS72)
        ts = new.epoch
        jd, fr = self._datetime_to_jdfr(ts)
        e, r, v = sat_prev.sgp4(jd, fr)
        if e != 0:
            return float("inf")
        sat_new = Satrec.twoline2rv(new.line1, new.line2, WGS72)
        e2, r2, _ = sat_new.sgp4(jd, fr)
        if e2 != 0:
            return float("inf")
        diff = sum((r2[i] - r[i]) ** 2 for i in range(3)) ** 0.5
        return diff

    @staticmethod
    def _datetime_to_jdfr(dt: datetime) -> Tuple[float, float]:
        jd = (dt - datetime(2000, 1, 1, tzinfo=timezone.utc)).total_seconds() / 86400.0 + 2451544.5
        jd_int = int(jd)
        fr = jd - jd_int
        return jd_int, fr


class TLEService:
    def __init__(self, repo: TLERepository, quality: TLEQuality, norad_id: int = 65488, source_cfg: TLESourceConfig = TLESourceConfig()):
        self.repo = repo
        self.quality = quality
        self.norad_id = norad_id
        self.source_cfg = source_cfg

    async def fetch_and_store(self) -> TLESet:
        tle = await self._fetch()
        previous = self.repo.latest(self.norad_id)
        tle = self.quality.assess(tle, previous)
        self.repo.save(tle)
        self._trim_history()
        return tle

    def _trim_history(self) -> None:
        # SQLite file stays small; we rely on queries to bound by days.
        pass

    async def _fetch(self) -> TLESet:
        now = datetime.now(timezone.utc)
        warnings: List[str] = []
        try:
            tle_lines = await self._fetch_from_official()
            source = "official"
            try:
                alt_lines = await self._fetch_from_celestrak(self.norad_id)
                warnings.extend(self._compare_sources(tle_lines, alt_lines))
            except Exception:
                warnings.append("CelesTrak fallback unavailable for consistency check")
        except Exception:
            try:
                tle_lines = await self._fetch_from_celestrak(self.norad_id)
                source = "celestrak"
            except Exception:
                # fallback mock TLE placeholder
                tle_lines = (
                    "1 25544U 98067A   25005.51000000  .00016717  00000+0  10270-3 0  9991",
                    "2 25544  51.6428  43.5905 0006786 306.3418  53.6891 15.50355749442124",
                )
                source = "mock"

        sat = Satrec.twoline2rv(tle_lines[0], tle_lines[1], WGS72)
        tle_epoch = self._sgp4_epoch_to_datetime(sat)
        return TLESet(
            norad_id=self.norad_id,
            source=source,
            fetched_at=now,
            epoch=tle_epoch,
            line1=tle_lines[0],
            line2=tle_lines[1],
            score=1.0,
            warnings=warnings,
        )

    async def _fetch_from_celestrak(self, norad_id: int) -> Tuple[str, str]:
        url = f"{self.source_cfg.fallback_url}?CATNR={norad_id}&FORMAT=TLE"
        async with httpx.AsyncClient(timeout=10) as client:
            resp = await client.get(url)
            resp.raise_for_status()
        lines = [ln.strip() for ln in resp.text.splitlines() if ln.strip().startswith(("1 ", "2 "))]
        if len(lines) < 2:
            raise RuntimeError("TLE not found")
        return lines[0], lines[1]

    async def _fetch_from_official(self) -> Tuple[str, str]:
        url = self.source_cfg.primary_url
        async with httpx.AsyncClient(timeout=10) as client:
            resp = await client.get(url)
            resp.raise_for_status()
        lines = [ln.strip() for ln in resp.text.splitlines() if ln.strip().startswith(("1 ", "2 "))]
        if len(lines) < 2:
            raise RuntimeError("Official TLE not found")
        return lines[0], lines[1]

    def _compare_sources(self, primary: Tuple[str, str], secondary: Tuple[str, str]) -> List[str]:
        warnings: List[str] = []
        primary_num = self._extract_satnum(primary[0])
        secondary_num = self._extract_satnum(secondary[0])
        if primary_num != secondary_num or primary_num != self.norad_id:
            warnings.append(f"NORAD mismatch primary={primary_num} secondary={secondary_num} expected={self.norad_id}")
            return warnings
        if primary != secondary:
            warnings.append("Official vs CelesTrak TLE lines differ")
        return warnings

    @staticmethod
    def _extract_satnum(line1: str) -> Optional[int]:
        try:
            return int(line1[2:7])
        except Exception:
            return None

    def select_for_time(self, t: datetime, history_days: int) -> Optional[TLESet]:
        candidates = self.repo.list_recent(self.norad_id, history_days)
        if not candidates:
            return None
        best = min(
            candidates,
            key=lambda tle: (
                abs((t - tle.epoch).total_seconds()),
                -tle.score,
            ),
        )
        return best

    @staticmethod
    def _sgp4_epoch_to_datetime(sat: Satrec) -> datetime:
        jd = sat.jdsatepoch + sat.jdsatepochF
        days = jd - 2440587.5
        seconds = days * 86400.0
        return datetime.fromtimestamp(seconds, tz=timezone.utc)
