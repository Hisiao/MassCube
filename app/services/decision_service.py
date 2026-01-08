from __future__ import annotations

from datetime import datetime, timedelta
from typing import Dict, List

from app.config import DecisionConfig
from app.models import FluxSample, RiskSample, TrackPoint, Window


class DecisionService:
    def __init__(self, cfg: DecisionConfig):
        self.cfg = cfg

    def compute_risk_series(self, flux_series: List[FluxSample]) -> List[RiskSample]:
        by_time: Dict[datetime, Dict[str, float]] = {}
        for sample in flux_series:
            by_time.setdefault(sample.t, {})[sample.channel] = sample.value

        risks: List[RiskSample] = []
        for t, channels in sorted(by_time.items()):
            risk_val = 0.0
            for ch, val in channels.items():
                weight = self.cfg.risk_weights.get(ch, 0.0)
                if val <= 0:
                    continue
                risk_val += weight * (val ** self.cfg.risk_exponent)
            risk_val *= self.cfg.risk_scale
            risks.append(RiskSample(t=t, risk=risk_val))
        return risks

    def decide_windows(self, track: List[TrackPoint], risks: List[RiskSample]) -> List[Window]:
        windows: List[Window] = []
        if not risks:
            return windows

        hold_until: datetime | None = None
        current_mode = "OBS_ON"
        last_change: datetime | None = None

        risk_by_time = {r.t: r for r in risks}
        for pt in track:
            risk = risk_by_time.get(pt.t)
            if not risk:
                continue
            thresholds = (self.cfg.r_off, self.cfg.r_on)
            risk_value = risk.risk

            # Inflate risk when orbit quality is degraded
            if pt.orbit_quality < 0.5:
                risk_value *= self.cfg.inflate_factor

            if current_mode == "OBS_ON" and risk_value > thresholds[0]:
                if self._can_change(pt.t, last_change, self.cfg.min_on_time_sec):
                    last_change = pt.t
                    current_mode = "OBS_OFF"
                    windows.append(
                        Window(
                            t_start=pt.t - timedelta(seconds=self.cfg.lead_time_sec),
                            t_end=pt.t,
                            mode="OBS_OFF",
                            reason=f"Risk {risk_value:.2f} > R_off {thresholds[0]}",
                            margins={"lead_time_sec": self.cfg.lead_time_sec, "lag_time_sec": self.cfg.lag_time_sec},
                        )
                    )
                    hold_until = pt.t + timedelta(seconds=self.cfg.hold_time_sec)
            elif current_mode == "OBS_OFF" and risk_value < thresholds[1]:
                if (hold_until and pt.t < hold_until) or not self._can_change(pt.t, last_change, self.cfg.min_off_time_sec):
                    continue
                last_change = pt.t
                current_mode = "OBS_ON"
                windows.append(
                    Window(
                        t_start=pt.t,
                        t_end=pt.t + timedelta(seconds=self.cfg.lag_time_sec),
                        mode="OBS_ON",
                        reason=f"Risk {risk_value:.2f} < R_on {thresholds[1]}",
                        margins={"lead_time_sec": self.cfg.lead_time_sec, "lag_time_sec": self.cfg.lag_time_sec},
                    )
                )
                hold_until = pt.t + timedelta(seconds=self.cfg.hold_time_sec)

        return windows

    @staticmethod
    def _can_change(now: datetime, last_change: datetime | None, min_interval: int) -> bool:
        if not last_change:
            return True
        return (now - last_change).total_seconds() >= min_interval
