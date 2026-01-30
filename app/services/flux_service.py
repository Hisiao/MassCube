from __future__ import annotations

import hashlib
import json
import subprocess
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Tuple

import numpy as np

from app.config import AE9AP9CLIConfig, AP8AE8Config, FluxConfig, FluxMockProfile
from app.models import FluxSample, TrackPoint


class FluxModel:
    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        raise NotImplementedError

    def flux_batch(self, points: List[TrackPoint], percentile: str) -> List[Dict[str, float]]:
        return [self.flux(pt, percentile) for pt in points]


class MockFluxModel(FluxModel):
    def __init__(self, profile: FluxMockProfile):
        self.profile = profile

    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        alt = point.alt_km
        frac = np.clip((alt - self.profile.quiet_altitude_km) / (self.profile.storm_altitude_km - self.profile.quiet_altitude_km), 0.0, 1.0)
        base = self.profile.quiet_flux + frac * (self.profile.storm_flux - self.profile.quiet_flux)
        noise = np.random.default_rng(seed=int(point.t.timestamp())).normal(1.0, 0.05)
        return {
            "Je>100keV": base * noise,
            "Je>1MeV": base * 0.5 * noise,
            "Jp>10MeV": base * 0.2 * noise,
            "Jp>50MeV": base * 0.05 * noise,
        }


class AE9AP9Model(FluxModel):
    def __init__(self, cfg: AE9AP9CLIConfig, channels: List[str]):
        self.cfg = cfg
        self.channels = channels

    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        return self.flux_batch([point], percentile)[0]

    def flux_batch(self, points: List[TrackPoint], percentile: str) -> List[Dict[str, float]]:
        if not self.cfg.executable or not self.cfg.command_template:
            raise RuntimeError("AE9/AP9 CLI not configured: set flux.ae9ap9.executable and command_template")
        payload = {
            "percentile": percentile,
            "channels": self.channels,
            "points": [
                {"lat": p.lat, "lon": p.lon, "alt_km": p.alt_km, "time": p.t.astimezone(timezone.utc).isoformat()}
                for p in points
            ],
        }
        data = self._run_cli(payload)
        return self._parse_output(data, len(points))

    def _run_cli(self, payload: Dict) -> Dict:
        cache_key = self._cache_key(payload)
        cached = self._read_cache(cache_key)
        if cached is not None:
            return cached
        with tempfile.TemporaryDirectory() as tmpdir:
            input_path = Path(tmpdir) / "input.json"
            output_path = Path(tmpdir) / "output.json"
            input_path.write_text(json.dumps(payload), encoding="utf-8")
            cmd = self.cfg.command_template.format(exe=self.cfg.executable, input=str(input_path), output=str(output_path))
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=self.cfg.timeout_sec)
            if result.returncode != 0:
                raise RuntimeError(f"IRENE CLI failed: {result.stderr.strip()}")
            if self.cfg.output_format == "json":
                data = json.loads(output_path.read_text(encoding="utf-8"))
            else:
                data = {"csv": output_path.read_text(encoding="utf-8")}
        self._write_cache(cache_key, data)
        return data

    def _parse_output(self, data: Dict, n_points: int) -> List[Dict[str, float]]:
        if "points" in data:
            values = []
            for item in data["points"]:
                values.append(item.get("values", {}))
            if len(values) != n_points:
                raise RuntimeError("IRENE output points count mismatch")
            return values
        if "csv" in data:
            return self._parse_csv(data["csv"], n_points)
        raise RuntimeError("IRENE output format not recognized")

    def _parse_csv(self, csv_text: str, n_points: int) -> List[Dict[str, float]]:
        values = [dict() for _ in range(n_points)]
        for line in csv_text.splitlines():
            parts = [p.strip() for p in line.split(",")]
            if len(parts) < 3:
                continue
            idx = int(parts[0])
            channel = parts[1]
            val = float(parts[2])
            if 0 <= idx < n_points:
                values[idx][channel] = val
        return values

    def _cache_key(self, payload: Dict) -> str:
        raw = json.dumps(payload, sort_keys=True).encode("utf-8")
        return hashlib.sha256(raw).hexdigest()

    def _read_cache(self, key: str) -> Dict | None:
        cache_dir = Path(self.cfg.cache_dir)
        cache_dir.mkdir(parents=True, exist_ok=True)
        path = cache_dir / f"{key}.json"
        if not path.exists():
            return None
        age = datetime.now(timezone.utc).timestamp() - path.stat().st_mtime
        if age > self.cfg.cache_ttl_sec:
            return None
        return json.loads(path.read_text(encoding="utf-8"))

    def _write_cache(self, key: str, data: Dict) -> None:
        cache_dir = Path(self.cfg.cache_dir)
        cache_dir.mkdir(parents=True, exist_ok=True)
        path = cache_dir / f"{key}.json"
        path.write_text(json.dumps(data), encoding="utf-8")


class AP8AE8Model(FluxModel):
    def __init__(self, cfg: AP8AE8Config):
        self.cfg = cfg
        self.channel = cfg.channel
        self.alt_km = cfg.alt_km
        self.latitudes, self.longitudes, self.grid = self._load_grid(cfg.pos_path, cfg.flux_path)

    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        return self.flux_batch([point], percentile)[0]

    def flux_batch(self, points: List[TrackPoint], percentile: str) -> List[Dict[str, float]]:
        values: List[Dict[str, float]] = []
        for pt in points:
            val = self._interp_flux(pt.lat, pt.lon)
            values.append({self.channel: val})
        return values

    def grid_values(self, channel: str) -> Tuple[List[float], List[float], List[List[float]]]:
        if channel != self.channel:
            empty = [[0.0 for _ in self.longitudes] for _ in self.latitudes]
            return list(self.latitudes), list(self.longitudes), empty
        return list(self.latitudes), list(self.longitudes), [row[:] for row in self.grid]

    def _interp_flux(self, lat: float, lon: float) -> float:
        lon = self._normalize_lon(lon)
        latitudes = np.asarray(self.latitudes, dtype=float)
        longitudes = np.asarray(self.longitudes, dtype=float)
        lat = float(np.clip(lat, latitudes[0], latitudes[-1]))
        lon = float(np.clip(lon, longitudes[0], longitudes[-1]))

        i1 = int(np.searchsorted(latitudes, lat, side="right"))
        if i1 <= 0:
            i0 = i1 = 0
        elif i1 >= len(latitudes):
            i0 = i1 = len(latitudes) - 1
        else:
            i0 = i1 - 1

        j1 = int(np.searchsorted(longitudes, lon, side="right"))
        if j1 <= 0:
            j0 = j1 = 0
        elif j1 >= len(longitudes):
            j0 = j1 = len(longitudes) - 1
        else:
            j0 = j1 - 1

        if i0 == i1 and j0 == j1:
            return self.grid[i0][j0]
        if i0 == i1:
            return self._lerp(self.grid[i0][j0], self.grid[i0][j1], longitudes[j0], longitudes[j1], lon)
        if j0 == j1:
            return self._lerp(self.grid[i0][j0], self.grid[i1][j0], latitudes[i0], latitudes[i1], lat)

        f00 = self.grid[i0][j0]
        f01 = self.grid[i0][j1]
        f10 = self.grid[i1][j0]
        f11 = self.grid[i1][j1]
        lon0 = longitudes[j0]
        lon1 = longitudes[j1]
        lat0 = latitudes[i0]
        lat1 = latitudes[i1]
        tx = 0.0 if lon1 == lon0 else (lon - lon0) / (lon1 - lon0)
        ty = 0.0 if lat1 == lat0 else (lat - lat0) / (lat1 - lat0)
        f0 = f00 + tx * (f01 - f00)
        f1 = f10 + tx * (f11 - f10)
        return f0 + ty * (f1 - f0)

    @staticmethod
    def _lerp(a: float, b: float, x0: float, x1: float, x: float) -> float:
        if x1 == x0:
            return a
        t = (x - x0) / (x1 - x0)
        return a + t * (b - a)

    @staticmethod
    def _normalize_lon(lon: float) -> float:
        while lon > 180.0:
            lon -= 360.0
        while lon < -180.0:
            lon += 360.0
        return lon

    def _load_grid(self, pos_path: str, flux_path: str) -> Tuple[List[float], List[float], List[List[float]]]:
        pos_lines = Path(pos_path).read_text(encoding="utf-8", errors="ignore").splitlines()
        flux_lines = Path(flux_path).read_text(encoding="utf-8", errors="ignore").splitlines()
        pos_start = self._find_data_start(pos_lines, "Universal Time")
        flux_start = self._find_data_start(flux_lines, "Flux")

        pos_rows = self._parse_rows(pos_lines[pos_start:], 5)
        flux_rows = self._parse_rows(flux_lines[flux_start:], 3)
        flux_values = [row[2] for row in flux_rows]

        if len(pos_rows) != len(flux_values):
            raise RuntimeError(f"SPENVIS grid mismatch: {len(pos_rows)} positions vs {len(flux_values)} flux values")

        latitudes: List[float] = []
        longitudes: List[float] = []
        grid: List[List[float]] = []
        current_lat = None
        row_values: List[float] = []
        row_lons: List[float] = []

        for (alt, lat, lon, _, _), flux in zip(pos_rows, flux_values):
            if current_lat is None or not np.isclose(lat, current_lat):
                if row_values:
                    if not longitudes:
                        longitudes = row_lons
                    elif len(row_lons) != len(longitudes) or not np.allclose(row_lons, longitudes):
                        raise RuntimeError("SPENVIS grid longitude rows mismatch")
                    grid.append(row_values)
                    row_values = []
                    row_lons = []
                current_lat = lat
                latitudes.append(lat)
            row_values.append(flux)
            row_lons.append(lon)

        if row_values:
            if not longitudes:
                longitudes = row_lons
            elif len(row_lons) != len(longitudes) or not np.allclose(row_lons, longitudes):
                raise RuntimeError("SPENVIS grid longitude rows mismatch")
            grid.append(row_values)

        if latitudes and latitudes[0] > latitudes[-1]:
            latitudes.reverse()
            grid.reverse()
        if longitudes and longitudes[0] > longitudes[-1]:
            longitudes.reverse()
            grid = [list(reversed(row)) for row in grid]

        return latitudes, longitudes, grid

    @staticmethod
    def _find_data_start(lines: List[str], marker: str) -> int:
        for idx, line in enumerate(lines):
            if marker in line:
                return idx + 1
        return 0

    @staticmethod
    def _parse_rows(lines: List[str], cols: int) -> List[List[float]]:
        rows: List[List[float]] = []
        for line in lines:
            line = line.strip()
            if not line or line.startswith(("*", "'")):
                continue
            parts = [p.strip() for p in line.split(",")]
            if len(parts) < cols:
                continue
            try:
                rows.append([float(p) for p in parts[:cols]])
            except ValueError:
                continue
        return rows


class FluxService:
    def __init__(self, cfg: FluxConfig):
        self.cfg = cfg
        self.model = self._build_model(cfg)
        if isinstance(self.model, AE9AP9Model):
            self.model_name = "ae9ap9"
        elif isinstance(self.model, AP8AE8Model):
            self.model_name = "ap8ae8"
        else:
            self.model_name = "mock"

    def _build_model(self, cfg: FluxConfig) -> FluxModel:
        if cfg.model == "mock":
            return MockFluxModel(cfg.mock_profile)
        if cfg.model == "ap8ae8":
            return AP8AE8Model(cfg.ap8ae8)
        channels = cfg.energy_channels if isinstance(cfg.energy_channels, list) else list(cfg.energy_channels.values())
        if not cfg.ae9ap9.executable or not cfg.ae9ap9.command_template:
            # Safe fallback to mock when AE9/AP9 CLI is not configured.
            return MockFluxModel(cfg.mock_profile)
        return AE9AP9Model(cfg.ae9ap9, channels)

    def compute_series(self, track: List[TrackPoint], percentile: str) -> List[FluxSample]:
        series: List[FluxSample] = []
        values_list = self.model.flux_batch(track, percentile)
        for pt, values in zip(track, values_list):
            for ch, val in values.items():
                series.append(FluxSample(t=pt.t, channel=ch, value=val, percentile=percentile))
        return series

    def compute_grid(
        self,
        t: datetime,
        channel: str,
        percentile: str,
        altitudes_km: List[float],
    ) -> Tuple[List[float], List[float], List[List[List[float]]]]:
        if isinstance(self.model, AP8AE8Model):
            lats, lons, grid = self.model.grid_values(channel)
            return lats, lons, [grid]
        cfg = self.cfg.ae9ap9
        t_bucketed = self._time_bucket(t, cfg.time_bucket_sec)
        latitudes = list(np.arange(-90.0, 90.0 + cfg.grid_lat_step_deg, cfg.grid_lat_step_deg))
        longitudes = list(np.arange(-180.0, 180.0 + cfg.grid_lon_step_deg, cfg.grid_lon_step_deg))
        values_3d: List[List[List[float]]] = []

        for alt in altitudes_km:
            points: List[TrackPoint] = []
            for lat in latitudes:
                for lon in longitudes:
                    points.append(
                        TrackPoint(
                            t=t_bucketed,
                            lat=float(lat),
                            lon=float(lon),
                            alt_km=float(alt),
                            tle_epoch=t,
                            orbit_quality=1.0,
                        )
                    )
            values = self._compute_grid_for_points(points, channel, percentile)
            grid = []
            idx = 0
            for _ in latitudes:
                row = values[idx : idx + len(longitudes)]
                grid.append(row)
                idx += len(longitudes)
            values_3d.append(grid)

        return latitudes, longitudes, values_3d

    def _compute_grid_for_points(self, points: List[TrackPoint], channel: str, percentile: str) -> List[float]:
        cfg = self.cfg.ae9ap9
        all_values: List[float] = []
        max_points = cfg.max_points_per_call if cfg.max_points_per_call > 0 else 2000
        for i in range(0, len(points), max_points):
            chunk = points[i : i + max_points]
            data = self.model.flux_batch(chunk, percentile)
            all_values.extend([vals.get(channel, 0.0) for vals in data])
        return all_values

    @staticmethod
    def _time_bucket(t: datetime, bucket_sec: int) -> datetime:
        if bucket_sec <= 0:
            return t
        ts = int(t.timestamp())
        bucket = ts - (ts % bucket_sec)
        return datetime.fromtimestamp(bucket, tz=timezone.utc)
