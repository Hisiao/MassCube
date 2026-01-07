from __future__ import annotations

from datetime import datetime, timedelta, timezone
from typing import List, Optional

import numpy as np
import pymap3d as pm
from sgp4.api import Satrec, WGS72

from app.config import OrbitConfig
from app.models import OrbitState, TrackPoint, TLESet


class OrbitService:
    def __init__(self, cfg: OrbitConfig):
        self.cfg = cfg

    def propagate(self, tle: TLESet, t: datetime) -> OrbitState:
        sat = Satrec.twoline2rv(tle.line1, tle.line2, WGS72)
        jd, fr = self._datetime_to_jdfr(t)
        error, r, v = sat.sgp4(jd, fr)
        if error != 0:
            raise RuntimeError(f"SGP4 error code {error}")
        lat, lon, alt = self._teme_to_geodetic(r, t)
        vel = np.linalg.norm(v)
        return OrbitState(
            t=t,
            lat=lat,
            lon=lon,
            alt_km=alt,
            velocity_km_s=vel,
            tle_epoch=tle.epoch,
            orbit_quality=tle.score,
        )

    def track(self, tle: TLESet, start: datetime, end: datetime, step_sec: int) -> List[TrackPoint]:
        points: List[TrackPoint] = []
        t = start
        while t <= end:
            state = self.propagate(tle, t)
            points.append(
                TrackPoint(
                    t=state.t,
                    lat=state.lat,
                    lon=state.lon,
                    alt_km=state.alt_km,
                    tle_epoch=state.tle_epoch,
                    orbit_quality=state.orbit_quality,
                )
            )
            t += timedelta(seconds=step_sec)
        return points

    def _teme_to_geodetic(self, r_teme: list[float], t: datetime) -> tuple[float, float, float]:
        if self.cfg.teme_to_ecef != "pymap3d":
            raise ValueError("Unsupported teme_to_ecef method")
        # Use pymap3d if available; fall back to GMST rotation if not.
        r_m = np.array(r_teme, dtype=float) * 1000.0
        if hasattr(pm, "teme2ecef"):
            x_ecef, y_ecef, z_ecef = pm.teme2ecef(r_m, t)
        elif hasattr(pm, "eci2ecef"):
            x_ecef, y_ecef, z_ecef = pm.eci2ecef(r_m[0], r_m[1], r_m[2], t)
        else:
            jd, fr = self._datetime_to_jdfr(t)
            gmst = self._gmst(jd + fr)
            x_teme, y_teme, z_teme = r_m
            x_ecef = x_teme * np.cos(gmst) - y_teme * np.sin(gmst)
            y_ecef = x_teme * np.sin(gmst) + y_teme * np.cos(gmst)
            z_ecef = z_teme
        lat, lon, alt = pm.ecef2geodetic(x_ecef, y_ecef, z_ecef, deg=True)
        lon = ((lon + 180.0) % 360.0) - 180.0  # normalize to [-180, 180]
        return float(lat), float(lon), float(alt / 1000.0)

    @staticmethod
    def _gmst(jd: float) -> float:
        """Greenwich mean sidereal time (radians) for TEME->ECEF fallback."""
        T = (jd - 2451545.0) / 36525.0
        gmst_deg = 280.46061837 + 360.98564736629 * (jd - 2451545.0) + 0.000387933 * T**2 - T**3 / 38710000.0
        return (gmst_deg % 360.0) * np.pi / 180.0

    @staticmethod
    def _datetime_to_jdfr(dt: datetime) -> tuple[float, float]:
        jd = (dt - datetime(2000, 1, 1, tzinfo=timezone.utc)).total_seconds() / 86400.0 + 2451544.5
        jd_int = int(jd)
        fr = jd - jd_int
        return jd_int, fr
