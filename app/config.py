from __future__ import annotations

import json
import os
from datetime import datetime
from pathlib import Path
from typing import Dict, Optional

import yaml
from pydantic import BaseModel, Field, ValidationError, field_validator

CONFIG_PATH = Path("config/config.yaml")
CONFIG_VERSION_LOG = Path("config/config_versions.json")


class OrbitConfig(BaseModel):
    refresh_rate_sec: int = 900
    prediction_horizon_sec: int = 4 * 3600
    history_days: int = 14
    teme_to_ecef: str = "pymap3d"
    track_step_sec: int = 60


class TLEQualityConfig(BaseModel):
    freshness_max_sec: int = 7200
    element_change_threshold: Dict[str, float] = Field(
        default_factory=lambda: {
            "mean_motion": 0.05,
            "inclination_deg": 0.05,
            "raan_deg": 0.05,
            "eccentricity": 0.0001,
        }
    )
    propagation_tolerance_km: float = 15.0
    score_weights: Dict[str, float] = Field(
        default_factory=lambda: {"freshness": 0.4, "stability": 0.3, "propagation": 0.3}
    )

    @field_validator("score_weights")
    @classmethod
    def validate_weights(cls, v: Dict[str, float]) -> Dict[str, float]:
        total = sum(v.values())
        if total == 0:
            raise ValueError("score_weights sum to zero")
        return {k: val / total for k, val in v.items()}


class FluxMockProfile(BaseModel):
    quiet_altitude_km: float = 800
    storm_altitude_km: float = 1200
    quiet_flux: float = 1.0e2
    storm_flux: float = 5.0e4


class FluxConfig(BaseModel):
    model: str = "mock"
    percentile_default: str = "mean"
    percentile_on_trigger: str = "p95"
    energy_channels: Dict[int, str] | list[str] = Field(
        default_factory=lambda: ["Je>100keV", "Je>1MeV", "Jp>10MeV", "Jp>50MeV"]
    )
    mock_profile: FluxMockProfile = FluxMockProfile()

    @field_validator("model")
    @classmethod
    def validate_model(cls, v: str) -> str:
        if v not in {"mock", "ae9ap9"}:
            raise ValueError("flux.model must be 'mock' or 'ae9ap9'")
        return v


class DecisionConfig(BaseModel):
    risk_weights: Dict[str, float] = Field(
        default_factory=lambda: {
            "Je>100keV": 0.4,
            "Je>1MeV": 0.3,
            "Jp>10MeV": 0.2,
            "Jp>50MeV": 0.1,
        }
    )
    r_off: float = 4.2
    r_on: float = 3.0
    hold_time_sec: int = 20
    min_off_time_sec: int = 0
    min_on_time_sec: int = 0
    lead_time_sec: int = 180
    lag_time_sec: int = 180
    inflate_factor: float = 2.0
    trigger_kp_threshold: int = 6

    @field_validator("risk_weights")
    @classmethod
    def normalize_weights(cls, v: Dict[str, float]) -> Dict[str, float]:
        total = sum(v.values())
        if total == 0:
            raise ValueError("decision.risk_weights sum to zero")
        return {k: val / total for k, val in v.items()}


class UIConfig(BaseModel):
    track_length_sec: int = 7200
    sampling_step_sec: int = 60
    risk_colorbar: Dict[str, float] = Field(default_factory=lambda: {"min": 0, "max": 6})


class AppConfig(BaseModel):
    orbits: OrbitConfig = OrbitConfig()
    tle_quality: TLEQualityConfig = TLEQualityConfig()
    flux: FluxConfig = FluxConfig()
    decision: DecisionConfig = DecisionConfig()
    ui: UIConfig = UIConfig()

    @classmethod
    def from_dict(cls, data: Dict) -> "AppConfig":
        return cls(**data)

    def model_dump_yaml(self) -> str:
        return yaml.safe_dump(self.model_dump(mode="json"), allow_unicode=False, sort_keys=False)


class ConfigStore:
    def __init__(self, config_path: Path = CONFIG_PATH, version_log: Path = CONFIG_VERSION_LOG):
        self.config_path = config_path
        self.version_log = version_log
        self.version: int = 0
        self.config: AppConfig = AppConfig()
        self.history: list[Dict] = []

    def load(self) -> AppConfig:
        if self.config_path.exists():
            with self.config_path.open("r", encoding="utf-8") as f:
                data = yaml.safe_load(f) or {}
                self.config = AppConfig.from_dict(data)
        self._load_history()
        return self.config

    def _load_history(self) -> None:
        if self.version_log.exists():
            with self.version_log.open("r", encoding="utf-8") as f:
                saved = json.load(f)
                self.history = saved.get("history", [])
                self.version = saved.get("version", len(self.history))
        else:
            self.history = []
            self.version = len(self.history)

    def _persist(self) -> None:
        self.config_path.parent.mkdir(parents=True, exist_ok=True)
        with self.config_path.open("w", encoding="utf-8") as f:
            f.write(self.config.model_dump_yaml())
        payload = {"version": self.version, "history": self.history}
        with self.version_log.open("w", encoding="utf-8") as f:
            json.dump(payload, f, default=str, indent=2)

    def update(self, config_dict: Dict, source: str = "api") -> AppConfig:
        try:
            new_cfg = AppConfig.from_dict(config_dict)
        except ValidationError as e:
            raise
        self.config = new_cfg
        self.version += 1
        entry = {"version": self.version, "applied_at": datetime.utcnow().isoformat(), "source": source, "config": config_dict}
        self.history.append(entry)
        self._persist()
        return self.config

    def rollback(self, version: Optional[int] = None) -> AppConfig:
        if not self.history:
            return self.config
        target = version if version is not None else (self.version - 1)
        selected = next((h for h in reversed(self.history) if h["version"] == target), None)
        if not selected:
            raise ValueError(f"version {target} not found")
        self.config = AppConfig.from_dict(selected["config"])
        self.version = selected["version"]
        self._persist()
        return self.config


def load_config() -> ConfigStore:
    store = ConfigStore()
    store.load()
    return store
