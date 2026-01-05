from __future__ import annotations

from datetime import datetime
from typing import Dict, List

import numpy as np

from app.config import DecisionConfig, FluxConfig, FluxMockProfile
from app.models import FluxSample, TrackPoint


class FluxModel:
    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        raise NotImplementedError


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
    def flux(self, point: TrackPoint, percentile: str) -> Dict[str, float]:
        # Placeholder for real IRENE adapter; keeps API stable for swaps.
        raise NotImplementedError("AE9/AP9 adapter not integrated; switch flux.model to 'mock'")


class FluxService:
    def __init__(self, cfg: FluxConfig):
        self.cfg = cfg
        self.model = self._build_model(cfg)

    def _build_model(self, cfg: FluxConfig) -> FluxModel:
        if cfg.model == "mock":
            return MockFluxModel(cfg.mock_profile)
        return AE9AP9Model()

    def compute_series(self, track: List[TrackPoint], percentile: str) -> List[FluxSample]:
        series: List[FluxSample] = []
        for pt in track:
            values = self.model.flux(pt, percentile)
            for ch, val in values.items():
                series.append(FluxSample(t=pt.t, channel=ch, value=val, percentile=percentile))
        return series
