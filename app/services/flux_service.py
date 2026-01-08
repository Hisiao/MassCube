from __future__ import annotations

import hashlib
import json
import subprocess
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Tuple

import numpy as np

from app.config import AE9AP9CLIConfig, FluxConfig, FluxMockProfile
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


class FluxService:
    def __init__(self, cfg: FluxConfig):
        self.cfg = cfg
        self.model = self._build_model(cfg)
        self.model_name = "ae9ap9" if isinstance(self.model, AE9AP9Model) else "mock"

    def _build_model(self, cfg: FluxConfig) -> FluxModel:
        if cfg.model == "mock":
            return MockFluxModel(cfg.mock_profile)
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
