export type OrbitState = {
  t: string;
  lat: number;
  lon: number;
  alt_km: number;
  velocity_km_s: number;
  tle_epoch: string;
  orbit_quality: number;
};

export type TrackPoint = {
  t: string;
  lat: number;
  lon: number;
  alt_km: number;
  tle_epoch: string;
  orbit_quality: number;
};

export type FluxSample = {
  t: string;
  channel: string;
  value: number;
  percentile: string;
};

export type Window = {
  t_start: string;
  t_end: string;
  mode: "OBS_ON" | "OBS_OFF";
  reason: string;
  margins: Record<string, number>;
};

export type OrbitConfig = {
  refresh_rate_sec: number;
  prediction_horizon_sec: number;
  history_days: number;
  teme_to_ecef: string;
  track_step_sec: number;
};

export type TLEQualityConfig = {
  freshness_max_sec: number;
  element_change_threshold: Record<string, number>;
  propagation_tolerance_km: number;
  score_weights: Record<string, number>;
};

export type FluxConfig = {
  model: "mock" | "ae9ap9";
  percentile_default: string;
  percentile_on_trigger: string;
  energy_channels: Record<number, string> | string[];
  mock_profile: {
    quiet_altitude_km: number;
    storm_altitude_km: number;
    quiet_flux: number;
    storm_flux: number;
  };
};

export type DecisionConfig = {
  risk_weights: Record<string, number>;
  r_off: number;
  r_on: number;
  hold_time_sec: number;
  min_off_time_sec: number;
  min_on_time_sec: number;
  lead_time_sec: number;
  lag_time_sec: number;
  inflate_factor: number;
  trigger_kp_threshold: number;
};

export type UIConfig = {
  track_length_sec: number;
  sampling_step_sec: number;
  risk_colorbar: Record<string, number>;
};

export type AppConfig = {
  orbits: OrbitConfig;
  tle_quality: TLEQualityConfig;
  flux: FluxConfig;
  decision: DecisionConfig;
  ui: UIConfig;
};

export type ConfigResponse = {
  version: number;
  config: AppConfig;
  history_size: number;
};
