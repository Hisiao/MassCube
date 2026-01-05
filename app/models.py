from __future__ import annotations

from datetime import datetime
from typing import Dict, List, Literal, Optional

from pydantic import BaseModel, Field


class TLESet(BaseModel):
    norad_id: int
    source: str
    fetched_at: datetime
    epoch: datetime
    line1: str
    line2: str
    score: float
    warnings: List[str] = Field(default_factory=list)


class OrbitState(BaseModel):
    t: datetime
    lat: float
    lon: float
    alt_km: float
    velocity_km_s: float
    tle_epoch: datetime
    orbit_quality: float


class TrackPoint(BaseModel):
    t: datetime
    lat: float
    lon: float
    alt_km: float
    tle_epoch: datetime
    orbit_quality: float


class FluxSample(BaseModel):
    t: datetime
    channel: str
    value: float
    percentile: str


class RiskSample(BaseModel):
    t: datetime
    risk: float
    reason: Optional[str] = None


class Window(BaseModel):
    t_start: datetime
    t_end: datetime
    mode: Literal["OBS_ON", "OBS_OFF"]
    reason: str
    margins: Dict[str, float] = Field(default_factory=dict)


class ConfigVersioned(BaseModel):
    version: int
    applied_at: datetime
    config: Dict
