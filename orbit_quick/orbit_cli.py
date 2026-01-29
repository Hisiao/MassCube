from __future__ import annotations

import argparse
import importlib
import os
from dataclasses import dataclass
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import Iterable, Optional, Tuple

import httpx
import numpy as np
import pymap3d as pm
from dateutil import parser as dateparser
from sgp4.api import Satrec, WGS72

CELESTRAK_URL = "https://celestrak.org/NORAD/elements/gp.php"
# http://8.141.94.91/tle/ty47.txt
# 65488
@dataclass(frozen=True)
class TLE:
    norad_id: int
    line1: str
    line2: str
    source: str


def _parse_time(value: Optional[str]) -> datetime:
    if not value:
        return datetime.now(timezone.utc)
    return dateparser.isoparse(value).astimezone(timezone.utc)


def _datetime_to_jdfr(dt: datetime) -> Tuple[float, float]:
    jd = (dt - datetime(2000, 1, 1, tzinfo=timezone.utc)).total_seconds() / 86400.0 + 2451544.5
    jd_int = int(jd)
    fr = jd - jd_int
    return jd_int, fr


def _datetime_to_mjd(dt: datetime) -> float:
    jd_int, fr = _datetime_to_jdfr(dt)
    return jd_int + fr - 2400000.5


def _gmst(jd: float) -> float:
    t = (jd - 2451545.0) / 36525.0
    gmst_deg = 280.46061837 + 360.98564736629 * (jd - 2451545.0) + 0.000387933 * t**2 - t**3 / 38710000.0
    return (gmst_deg % 360.0) * np.pi / 180.0


def _teme_to_geodetic(r_teme: list[float], t: datetime) -> tuple[float, float, float]:
    r_m = np.array(r_teme, dtype=float) * 1000.0
    if hasattr(pm, "teme2ecef"):
        x_ecef, y_ecef, z_ecef = pm.teme2ecef(r_m, t)
    elif hasattr(pm, "eci2ecef"):
        x_ecef, y_ecef, z_ecef = pm.eci2ecef(r_m[0], r_m[1], r_m[2], t)
    else:
        jd, fr = _datetime_to_jdfr(t)
        gmst = _gmst(jd + fr)
        x_teme, y_teme, z_teme = r_m
        x_ecef = x_teme * np.cos(gmst) - y_teme * np.sin(gmst)
        y_ecef = x_teme * np.sin(gmst) + y_teme * np.cos(gmst)
        z_ecef = z_teme
    lat, lon, alt = pm.ecef2geodetic(x_ecef, y_ecef, z_ecef, deg=True)
    lon = ((lon + 180.0) % 360.0) - 180.0
    return float(lat), float(lon), float(alt / 1000.0)


def _extract_tle_from_text(text: str, norad_id: int) -> Tuple[str, str]:
    lines = [ln.strip() for ln in text.splitlines() if ln.strip()]
    for idx in range(len(lines) - 1):
        if not (lines[idx].startswith("1 ") and lines[idx + 1].startswith("2 ")):
            continue
        try:
            satnum = int(lines[idx][2:7])
        except Exception:
            continue
        if satnum == norad_id:
            return lines[idx], lines[idx + 1]
    # Fallback: if there is only one TLE in the response
    tle_lines = [ln for ln in lines if ln.startswith(("1 ", "2 "))]
    if len(tle_lines) >= 2:
        return tle_lines[0], tle_lines[1]
    raise RuntimeError("TLE not found in response")


def fetch_tle(
    norad_id: int,
    official_url: Optional[str],
    official_user: Optional[str],
    official_pass: Optional[str],
    celestrak_url: str = CELESTRAK_URL,
) -> TLE:
    if official_url:
        try:
            if "{norad_id}" in official_url:
                official_url = official_url.format(norad_id=norad_id)
            auth = None
            if official_user and official_pass:
                auth = (official_user, official_pass)
            with httpx.Client(timeout=15, auth=auth) as client:
                resp = client.get(official_url)
                resp.raise_for_status()
            line1, line2 = _extract_tle_from_text(resp.text, norad_id)
            return TLE(norad_id=norad_id, line1=line1, line2=line2, source="official")
        except Exception:
            pass

    url = f"{celestrak_url}?CATNR={norad_id}&FORMAT=TLE"
    with httpx.Client(timeout=15) as client:
        resp = client.get(url)
        resp.raise_for_status()
    line1, line2 = _extract_tle_from_text(resp.text, norad_id)
    return TLE(norad_id=norad_id, line1=line1, line2=line2, source="celestrak")


def iter_track(tle: TLE, start: datetime, end: datetime, step_sec: int) -> Iterable[tuple[datetime, float, float, float]]:
    sat = Satrec.twoline2rv(tle.line1, tle.line2, WGS72)
    t = start
    while t <= end:
        jd, fr = _datetime_to_jdfr(t)
        error, r, _ = sat.sgp4(jd, fr)
        if error != 0:
            raise RuntimeError(f"SGP4 error code {error}")
        lat, lon, alt = _teme_to_geodetic(r, t)
        yield t, lat, lon, alt
        t += timedelta(seconds=step_sec)


def _format_time_ms(t: datetime) -> str:
    return t.strftime("%d %b %Y %H:%M:%S.%f")[:-3]


def _compute_ae8_ap8_flux(
    track: list[tuple[datetime, float, float, float]],
) -> tuple[np.ndarray, np.ndarray]:
    backend_module = os.getenv("AE8AP8_BACKEND_MODULE", "ae8ap8_irbem")
    module = importlib.import_module(backend_module)
    if not hasattr(module, "compute_ae8_ap8_flux"):
        raise RuntimeError(f"AE8/AP8 backend '{backend_module}' missing compute_ae8_ap8_flux()")

    return module.compute_ae8_ap8_flux(track, 0.650, 1.500)


def write_output(
    path: Path,
    track: Iterable[tuple[datetime, float, float, float]],
    ele_flux: Optional[Iterable[float]] = None,
    pro_flux: Optional[Iterable[float]] = None,
) -> None:
    columns = [
        ("Time (UTC)", 24),
        ("Detic Latitude (deg)", 20),
        ("Detic Longitude (deg)", 21),
        ("Detic Altitude (km)", 19),
        ("Ele Flux (0.650 MeV) (m^-2*sec^-1*MeV^-1)", 42),
        ("Pro Flux (1.500 MeV) (m^-2*sec^-1*MeV^-1)", 42),
    ]
    header = (
        "    ".join(f"{name:>{width}}" for name, width in columns) + "\n" +
        "    ".join("-" * width for _, width in columns) + "\n"
    )
    with path.open("w", encoding="ascii") as f:
        f.write(header)
        if ele_flux is None or pro_flux is None:
            for t, lat, lon, alt in track:
                line = f"{_format_time_ms(t):>24}    {lat:>20.3f}    {lon:>21.3f}    {alt:>19.3f}\n"
                f.write(line)
        else:
            for (t, lat, lon, alt), ele, pro in zip(track, ele_flux, pro_flux):
                line = (
                    f"{_format_time_ms(t):>24}    {lat:>20.3f}    {lon:>21.3f}    {alt:>19.3f}"
                    f"    {ele:>42.6e}    {pro:>42.6e}\n"
                )
                f.write(line)


def _resolve_output_path(out: Optional[str], start: datetime) -> Path:
    if not out:
        name = start.strftime("%Y%m%d%H") + ".txt"
        return Path.cwd() / name
    path = Path(out)
    if path.exists() and path.is_dir():
        name = start.strftime("%Y%m%d%H") + ".txt"
        return path / name
    if out.endswith(("/", "\\")):
        path.mkdir(parents=True, exist_ok=True)
        name = start.strftime("%Y%m%d%H") + ".txt"
        return path / name
    path.parent.mkdir(parents=True, exist_ok=True)
    return path


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Quick TLE -> one-day ground track (lat/lon/alt).")
    parser.add_argument("--norad-id", type=int, required=True, help="NORAD catalog number")
    parser.add_argument("--start", type=str, default=None, help="Start time (ISO-8601, default now UTC)")
    parser.add_argument("--hours", type=int, default=24, help="Prediction horizon hours (default 24)")
    parser.add_argument("--step", type=int, default=10, help="Step size in seconds (default 10)")
    parser.add_argument("--out", type=str, default=None, help="Output file path or directory")
    parser.add_argument("--official-url", type=str, default=None, help="Official TLE URL (optional)")
    parser.add_argument("--official-user", type=str, default=None, help="Official source username")
    parser.add_argument("--official-pass", type=str, default=None, help="Official source password")
    parser.add_argument("--celestrak-url", type=str, default=CELESTRAK_URL, help="CelesTrak base URL")
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    start = _parse_time(args.start)
    end = start + timedelta(hours=args.hours)
    out_path = _resolve_output_path(args.out, start)

    tle = fetch_tle(
        norad_id=args.norad_id,
        official_url=args.official_url,
        official_user=args.official_user,
        official_pass=args.official_pass,
        celestrak_url=args.celestrak_url,
    )

    track = list(iter_track(tle, start, end, args.step))
    ele_flux, pro_flux = _compute_ae8_ap8_flux(track)
    write_output(out_path, track, ele_flux, pro_flux)

    print(f"TLE source: {tle.source}")
    print(f"Output: {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
