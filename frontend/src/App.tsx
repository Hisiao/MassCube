import { useEffect, useMemo, useState } from "react";
import {
  Cartesian3,
  Color,
  GridImageryProvider,
  ImageMaterialProperty,
  Rectangle,
  Viewer as CesiumViewer,
} from "cesium";
import { ImageryLayer, Viewer, Entity } from "resium";
import {
  getConfig,
  exportObservationPlan,
  getFlux,
  getFluxGrid,
  getState,
  getTrack,
  getWindows,
  updateConfig,
} from "./api";
import type {
  AppConfig,
  ConfigResponse,
  FluxGrid,
  FluxSample,
  OrbitState,
  TrackPoint,
  Window,
} from "./types";

type RiskPoint = { t: string; risk: number };
type SmoothPos = { t: string; lat: number; lon: number; alt_km: number };

const fmt = (v: number, digits = 3) => v.toFixed(digits);
const toIso = (d: Date) => d.toISOString();
const formatFlux = (v: number) => (v > 0 ? v.toExponential(2) : "0");
const formatUtc = (iso: string) => new Date(iso).toISOString().replace("T", " ").slice(0, 19) + "Z";

function normalizeLon(lon: number) {
  return ((lon + 180) % 360) - 180;
}

function lerp(a: number, b: number, t: number) {
  return a + (b - a) * t;
}

function lerpLon(a: number, b: number, t: number) {
  let delta = b - a;
  if (Math.abs(delta) > 180) {
    delta = delta - Math.sign(delta) * 360;
  }
  return normalizeLon(a + delta * t);
}

function bucketRisk(r: number): "low" | "med" | "high" {
  if (r >= 5) return "high";
  if (r >= 3) return "med";
  return "low";
}

function colorForRisk(r: number) {
  const bucket = bucketRisk(r);
  if (bucket === "high") return Color.fromCssColorString("#f97316");
  if (bucket === "med") return Color.fromCssColorString("#22d3ee");
  return Color.fromCssColorString("#a3e635");
}

function useInterval(callback: () => void, delayMs: number) {
  useEffect(() => {
    const id = setInterval(callback, delayMs);
    return () => clearInterval(id);
  }, [callback, delayMs]);
}

export default function App() {
  const [configResp, setConfigResp] = useState<ConfigResponse | null>(null);
  const [cfgDraft, setCfgDraft] = useState<AppConfig | null>(null);
  const [state, setState] = useState<OrbitState | null>(null);
  const [track, setTrack] = useState<TrackPoint[]>([]);
  const [flux, setFlux] = useState<FluxSample[]>([]);
  const [fluxGrid, setFluxGrid] = useState<FluxGrid | null>(null);
  const [fluxOverlay, setFluxOverlay] = useState<string | null>(null);
  const [fluxChannel, setFluxChannel] = useState<string>("");
  const [fluxStats, setFluxStats] = useState<{ min: number; max: number } | null>(null);
  const [windows, setWindows] = useState<Window[]>([]);
  const [riskSeries, setRiskSeries] = useState<RiskPoint[]>([]);
  const [smoothPos, setSmoothPos] = useState<SmoothPos | null>(null);
  const [mode, setMode] = useState<{ mode: "OBS_ON" | "OBS_OFF"; reason?: string }>({ mode: "OBS_ON" });
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [planStart, setPlanStart] = useState<string>(() => new Date().toISOString().slice(0, 19) + "Z");

  const gridProvider = useMemo(
    () =>
      new GridImageryProvider({
        cells: 18,
        color: Color.WHITE.withAlpha(0.35),
        glowColor: Color.WHITE.withAlpha(0.0),
        backgroundColor: Color.BLACK.withAlpha(0.0),
      }),
    []
  );

  const fineGridProvider = useMemo(
    () =>
      new GridImageryProvider({
        cells: 72,
        color: Color.WHITE.withAlpha(0.12),
        glowColor: Color.WHITE.withAlpha(0.0),
        backgroundColor: Color.BLACK.withAlpha(0.0),
      }),
    []
  );

  const latLabels = useMemo(() => {
    const labels: { text: string; lat: number; lon: number }[] = [];
    for (let lat = -60; lat <= 60; lat += 30) {
      const suffix = lat === 0 ? "" : lat > 0 ? "N" : "S";
      labels.push({ text: `${Math.abs(lat)}°${suffix}`, lat, lon: 179 });
    }
    return labels;
  }, []);

  const lonLabels = useMemo(() => {
    const labels: { text: string; lat: number; lon: number }[] = [];
    for (let lon = -150; lon <= 150; lon += 30) {
      const suffix = lon === 0 ? "" : lon > 0 ? "E" : "W";
      labels.push({ text: `${Math.abs(lon)}°${suffix}`, lat: 0, lon });
    }
    return labels;
  }, []);

  useEffect(() => {
    (async () => {
      try {
        const resp = await getConfig();
        setConfigResp(resp);
        setCfgDraft(resp.config);
      } catch (err) {
        setError(String(err));
      }
    })();
  }, []);

  const refreshData = async () => {
    if (!configResp) return;
    setLoading(true);
    setError(null);
    const now = new Date();
    const start = new Date(now.getTime() - 90 * 60 * 1000);
    const end = new Date(now.getTime() + 90 * 60 * 1000);
    const step = configResp.config.orbits.track_step_sec;
    try {
      const [st, trk, flx, win] = await Promise.all([
        getState(toIso(now)),
        getTrack(toIso(start), toIso(end), step),
        getFlux(toIso(start), toIso(end), step, configResp.config.flux.percentile_default),
        getWindows(toIso(start), toIso(end), step, configResp.config.flux.percentile_default),
      ]);
      setState(st);
      setTrack(trk);
      setFlux(flx);
      setWindows(win);
      const risks = computeRiskSeries(
        flx,
        configResp.config.decision.risk_weights,
        configResp.config.decision.risk_exponent,
        configResp.config.decision.risk_scale
      );
      setRiskSeries(risks);
      setMode(currentMode(win, now));
      await refreshGrid(now);
    } catch (err) {
      setError(String(err));
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    if (configResp) {
      refreshData();
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [configResp]);

  useInterval(() => {
    refreshData();
  }, 15000);

  useInterval(() => {
    refreshGrid(new Date());
  }, 60000);

  const channelOptions = useMemo(() => {
    const ch = configResp?.config.flux.energy_channels;
    if (!ch) return [];
    return Array.isArray(ch) ? ch : Object.values(ch);
  }, [configResp]);

  useEffect(() => {
    if (!fluxChannel && channelOptions.length) {
      setFluxChannel(channelOptions[0]);
    }
  }, [fluxChannel, channelOptions]);

  useEffect(() => {
    if (configResp && fluxChannel) {
      refreshGrid(new Date());
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [configResp, fluxChannel]);

  useEffect(() => {
    if (!track.length) return;
    const update = () => {
      const pos = interpolateTrackPosition(track, new Date());
      if (pos) setSmoothPos(pos);
    };
    update();
    const id = setInterval(update, 500);
    return () => clearInterval(id);
  }, [track]);

  const riskByTime = useMemo(() => {
    const map = new Map<string, number>();
    riskSeries.forEach((r) => map.set(r.t, r.risk));
    return map;
  }, [riskSeries]);

  const { pastSegments, futureSegments } = useMemo(() => {
    if (!track.length) return { pastSegments: [], futureSegments: [] };
    const nowTs = Date.now();
    const past = track.filter((pt) => new Date(pt.t).getTime() <= nowTs);
    const future = track.filter((pt) => new Date(pt.t).getTime() > nowTs);
    const buildSegments = (pts: TrackPoint[], alpha: number) => {
      const segments: { positions: Cartesian3[]; color: Color }[] = [];
      let current: { positions: Cartesian3[]; color: Color } | null = null;
      let lastPos: Cartesian3 | null = null;
      for (const pt of pts) {
        const risk = riskByTime.get(pt.t) ?? 0;
        const color = colorForRisk(risk).withAlpha(alpha);
        const pos = Cartesian3.fromDegrees(normalizeLon(pt.lon), pt.lat, pt.alt_km * 1000);
        if (!current || !current.color.equals(color)) {
          current = { positions: [], color };
          if (lastPos) current.positions.push(lastPos);
          current.positions.push(pos);
          segments.push(current);
        } else {
          current.positions.push(pos);
        }
        lastPos = pos;
      }
      return segments;
    };
    return {
      pastSegments: buildSegments(past, 0.95),
      futureSegments: buildSegments(future, 0.6),
    };
  }, [track, riskByTime]);

  const currentTleInfo = useMemo(() => {
    if (!state) return null;
    return {
      epoch: state.tle_epoch,
      quality: state.orbit_quality,
    };
  }, [state]);

  const applyConfig = async () => {
    if (!cfgDraft) return;
    try {
      const resp = await updateConfig(cfgDraft);
      setConfigResp(resp);
      setCfgDraft(resp.config);
      await refreshData();
    } catch (err) {
      setError(String(err));
    }
  };

  const resetDraft = () => {
    if (configResp) setCfgDraft(configResp.config);
  };

  const handleCfgChange = <K extends keyof AppConfig>(key: K, val: AppConfig[K]) => {
    setCfgDraft((prev) => (prev ? { ...prev, [key]: val } : prev));
  };

  const downloadPlan = async () => {
    try {
      const blob = await exportObservationPlan(planStart, 24);
      const url = URL.createObjectURL(blob);
      const a = document.createElement("a");
      a.href = url;
      a.download = "observation_plan.csv";
      a.click();
      URL.revokeObjectURL(url);
    } catch (err) {
      setError(String(err));
    }
  };

  const refreshGrid = async (now: Date) => {
    if (!configResp || !fluxChannel) return;
    const alt = configResp.config.flux.ae9ap9?.default_alt_km;
    try {
      const grid = await getFluxGrid(toIso(now), fluxChannel, configResp.config.flux.percentile_default, alt);
      setFluxGrid(grid);
      setFluxOverlay(buildHeatmapOverlay(grid));
      setFluxStats(computeFluxStats(grid));
    } catch (err) {
      setError(String(err));
    }
  };

  return (
    <div className="app-shell">
      <aside className="sidebar">
        <div className="panel">
          <h3 title="当前实时状态">实时状态</h3>
          {state ? (
            <div className="state-grid">
              <div className="state-item">
                <label>时间 (UTC)</label>
                <span>{new Date(state.t).toISOString()}</span>
              </div>
              <div className="state-item">
                <label>模式</label>
                <span>{mode.mode}</span>
              </div>
              <div className="state-item">
                <label>纬度</label>
                <span>{fmt(state.lat, 3)}°</span>
              </div>
              <div className="state-item">
                <label>经度</label>
                <span>{fmt(state.lon, 3)}°</span>
              </div>
              <div className="state-item">
                <label>高度</label>
                <span>{fmt(state.alt_km, 1)} km</span>
              </div>
              <div className="state-item">
                <label>速度</label>
                <span>{fmt(state.velocity_km_s, 2)} km/s</span>
              </div>
              <div className="state-item">
                <label>TLE epoch</label>
                <span>{currentTleInfo?.epoch}</span>
              </div>
              <div className="state-item">
                <label>轨道质量</label>
                <span>{fmt(currentTleInfo?.quality ?? 0, 2)}</span>
              </div>
              {mode.mode === "OBS_OFF" && mode.reason && (
                <div className="state-item" style={{ gridColumn: "span 2" }}>
                  <label>原因</label>
                  <span>{mode.reason}</span>
                </div>
              )}
            </div>
          ) : (
            <div className="status">{loading ? "加载中..." : "暂无数据"}</div>
          )}
        </div>

        <div className="panel">
          <h3 title="global flux map">global flux map</h3>
          <div className="form-grid">
            <label>
              channel
              <select value={fluxChannel} onChange={(e) => setFluxChannel(e.target.value)}>
                {channelOptions.map((ch) => (
                  <option key={ch} value={ch}>
                    {ch}
                  </option>
                ))}
              </select>
            </label>
          </div>
          <div className="status">{fluxGrid ? `grid @ ${new Date(fluxGrid.t).toISOString()}` : "grid pending"}</div>
        </div>

        <div className="panel">
          <h3 title="导出未来 24h 观测计划">导出观测计划</h3>
          <div className="form-grid">
            <label>
              start (UTC)
              <input
                type="text"
                value={planStart}
                onChange={(e) => setPlanStart(e.target.value)}
                placeholder="2026-01-05T15:00:00Z"
              />
            </label>
          </div>
          <div className="btn-row">
            <button className="btn primary" onClick={downloadPlan}>
              导出 CSV
            </button>
          </div>
          <div className="status">输出未来 24h 观测计划（OBS_ON 时段）</div>
        </div>

        <div className="panel">
          <h3 title="调整并应用所有风险与时序阈值">参数配置</h3>
          {cfgDraft ? (
            <>
              <div className="form-grid">
                <label title="风险阈值，高于此值关闭观测">
                  R_off
                  <input
                    type="number"
                    value={cfgDraft.decision.r_off}
                    onChange={(e) => handleCfgChange("decision", { ...cfgDraft.decision, r_off: Number(e.target.value) })}
                  />
                </label>
                <label title="风险阈值，低于此值恢复观测">
                  R_on
                  <input
                    type="number"
                    value={cfgDraft.decision.r_on}
                    onChange={(e) => handleCfgChange("decision", { ...cfgDraft.decision, r_on: Number(e.target.value) })}
                  />
                </label>
                <label title="提前关机时间（秒）">
                  lead_time_sec
                  <input
                    type="number"
                    value={cfgDraft.decision.lead_time_sec}
                    onChange={(e) => handleCfgChange("decision", { ...cfgDraft.decision, lead_time_sec: Number(e.target.value) })}
                  />
                </label>
                <label title="延后开机时间（秒）">
                  lag_time_sec
                  <input
                    type="number"
                    value={cfgDraft.decision.lag_time_sec}
                    onChange={(e) => handleCfgChange("decision", { ...cfgDraft.decision, lag_time_sec: Number(e.target.value) })}
                  />
                </label>
                <label title="最小关闭保持时间（秒）">
                  hold_time_sec
                  <input
                    type="number"
                    value={cfgDraft.decision.hold_time_sec}
                    onChange={(e) =>
                      handleCfgChange("decision", { ...cfgDraft.decision, hold_time_sec: Number(e.target.value) })
                    }
                  />
                </label>
                <label title="最小关闭持续（秒）">
                  min_off_time_sec
                  <input
                    type="number"
                    value={cfgDraft.decision.min_off_time_sec}
                    onChange={(e) =>
                      handleCfgChange("decision", { ...cfgDraft.decision, min_off_time_sec: Number(e.target.value) })
                    }
                  />
                </label>
                <label title="最小开启持续（秒）">
                  min_on_time_sec
                  <input
                    type="number"
                    value={cfgDraft.decision.min_on_time_sec}
                    onChange={(e) =>
                      handleCfgChange("decision", { ...cfgDraft.decision, min_on_time_sec: Number(e.target.value) })
                    }
                  />
                </label>
                <label title="轨道质量差时风险放大系数">
                  inflate_factor
                  <input
                    type="number"
                    value={cfgDraft.decision.inflate_factor}
                    step="0.1"
                    onChange={(e) =>
                      handleCfgChange("decision", { ...cfgDraft.decision, inflate_factor: Number(e.target.value) })
                    }
                  />
                </label>
                <label title="默认分位（Mean/P95）">
                  分位
                  <select
                    value={cfgDraft.flux.percentile_default}
                    onChange={(e) => handleCfgChange("flux", { ...cfgDraft.flux, percentile_default: e.target.value })}
                  >
                    <option value="mean">Mean</option>
                    <option value="p95">P95</option>
                  </select>
                </label>
                <label title="刷新步长（秒）">
                  track_step_sec
                  <input
                    type="number"
                    value={cfgDraft.orbits.track_step_sec}
                    onChange={(e) =>
                      handleCfgChange("orbits", { ...cfgDraft.orbits, track_step_sec: Number(e.target.value) })
                    }
                  />
                </label>
              </div>

              <div style={{ marginTop: 8, fontSize: 12, color: "#9ca3af" }}>
                风险权重 (0-1，会自动归一化)
              </div>
              <div className="form-grid">
                {Object.entries(cfgDraft.decision.risk_weights).map(([ch, w]) => (
                  <label key={ch} title={`${ch} 权重`}>
                    {ch}
                    <input
                      type="number"
                      step="0.05"
                      value={w}
                      onChange={(e) =>
                        handleCfgChange("decision", {
                          ...cfgDraft.decision,
                          risk_weights: { ...cfgDraft.decision.risk_weights, [ch]: Number(e.target.value) },
                        })
                      }
                    />
                  </label>
                ))}
              </div>

              <div className="btn-row">
                <button className="btn primary" onClick={applyConfig} title="PUT /config 并重载数据">
                  保存并应用
                </button>
                <button className="btn secondary" onClick={resetDraft} title="撤销未保存修改">
                  取消修改
                </button>
              </div>
              <div className="status">当前版本：{configResp?.version ?? "-"}（history {configResp?.history_size ?? 0}）</div>
            </>
          ) : (
            <div className="status">{loading ? "加载配置..." : "无法加载配置"}</div>
          )}
        </div>

        {error && <div className="error">错误：{error}</div>}
        <div className="status">{loading ? "数据刷新中..." : "实时刷新每 15 秒"}</div>
      </aside>

      <section className="viewer">
        <Viewer
          style={{ width: "100%", height: "100%" }}
          timeline={false}
          animation={false}
          sceneModePicker={false}
          baseLayerPicker={false}
          navigationHelpButton={false}
          homeButton={false}
          geocoder={false}
        >
          <ImageryLayer imageryProvider={fineGridProvider} alpha={0.5} />
          <ImageryLayer imageryProvider={gridProvider} alpha={0.7} />
          {fluxOverlay && (
            <Entity
              rectangle={{
                coordinates: Rectangle.fromDegrees(-180, -90, 180, 90),
                material: new ImageMaterialProperty({ image: fluxOverlay, transparent: true }),
              }}
            />
          )}
          {latLabels.map((l) => (
            <Entity
              key={`lat-${l.lat}`}
              position={Cartesian3.fromDegrees(l.lon, l.lat, 0)}
              label={{
                text: l.text,
                font: "11px sans-serif",
                fillColor: Color.WHITE.withAlpha(0.7),
                outlineColor: Color.BLACK.withAlpha(0.4),
                outlineWidth: 1,
                showBackground: false,
              }}
            />
          ))}
          {lonLabels.map((l) => (
            <Entity
              key={`lon-${l.lon}`}
              position={Cartesian3.fromDegrees(l.lon, l.lat, 0)}
              label={{
                text: l.text,
                font: "11px sans-serif",
                fillColor: Color.WHITE.withAlpha(0.7),
                outlineColor: Color.BLACK.withAlpha(0.4),
                outlineWidth: 1,
                showBackground: false,
              }}
            />
          ))}
          {pastSegments.map((seg, idx) => (
            <Entity
              key={`past-${idx}`}
              polyline={{
                positions: seg.positions,
                width: 3,
                material: seg.color,
              }}
            />
          ))}
          {futureSegments.map((seg, idx) => (
            <Entity
              key={`future-${idx}`}
              polyline={{
                positions: seg.positions,
                width: 2,
                material: seg.color,
              }}
            />
          ))}
          {(smoothPos || state) && (
            <Entity
              name="satellite"
              position={Cartesian3.fromDegrees(
                normalizeLon((smoothPos ?? state)!.lon),
                (smoothPos ?? state)!.lat,
                (smoothPos ?? state)!.alt_km * 1000
              )}
              point={{ pixelSize: 10, color: Color.CYAN, outlineColor: Color.WHITE, outlineWidth: 2 }}
              description={`Lat: ${fmt((smoothPos ?? state)!.lat)}, Lon: ${fmt(
                (smoothPos ?? state)!.lon
              )}, Alt: ${fmt((smoothPos ?? state)!.alt_km)} km`}
            />
          )}
        </Viewer>
        <div className="legend">
          <div className="legend-row">
            <div className="legend-swatch" style={{ background: "#a3e635" }}></div>
            <span>低风险</span>
          </div>
          <div className="legend-row">
            <div className="legend-swatch" style={{ background: "#22d3ee" }}></div>
            <span>中风险</span>
          </div>
          <div className="legend-row">
            <div className="legend-swatch" style={{ background: "#f97316" }}></div>
            <span>高风险</span>
          </div>
        </div>
        {fluxStats && (
          <div className="legend flux-legend">
            <div className="legend-title">flux density</div>
            <div className="legend-unit">(cm²·s·sr·MeV)⁻¹</div>
            <div className="legend-vert">
              <div className="legend-bar-vert" />
              <div className="legend-scale-vert">
                <span>{formatFlux(fluxStats.max)}</span>
                <span>{formatFlux(fluxStats.min)}</span>
              </div>
            </div>
            <div className="legend-note">{fluxChannel || "channel"}</div>
          </div>
        )}
      </section>
    </div>
  );
}

function computeRiskSeries(
  samples: FluxSample[],
  weights: Record<string, number>,
  exponent: number,
  scale: number
): RiskPoint[] {
  const grouped = new Map<string, Record<string, number>>();
  for (const s of samples) {
    const key = s.t;
    const channels = grouped.get(key) ?? {};
    channels[s.channel] = s.value;
    grouped.set(key, channels);
  }
  const res: RiskPoint[] = [];
  for (const [t, channels] of grouped.entries()) {
    let risk = 0;
    for (const [ch, val] of Object.entries(channels)) {
      const w = weights[ch] ?? 0;
      if (val > 0) risk += w * Math.pow(val, exponent);
    }
    res.push({ t, risk: risk * scale });
  }
  return res.sort((a, b) => new Date(a.t).getTime() - new Date(b.t).getTime());
}

function currentMode(windows: Window[], now: Date): { mode: "OBS_ON" | "OBS_OFF"; reason?: string } {
  let mode: "OBS_ON" | "OBS_OFF" = "OBS_ON";
  let reason: string | undefined;
  for (const w of windows) {
    const start = new Date(w.t_start).getTime();
    const end = new Date(w.t_end).getTime();
    if (now.getTime() >= start && now.getTime() <= end) {
      mode = w.mode;
      reason = w.reason;
      break;
    }
    if (now.getTime() > end) {
      mode = w.mode;
      reason = w.reason;
    }
  }
  return { mode, reason };
}

function buildHeatmapOverlay(grid: FluxGrid): string | null {
  if (!grid.values.length) return null;
  const layer = grid.values[0];
  const height = layer.length;
  const width = layer[0]?.length ?? 0;
  if (!width || !height) return null;

  const values = layer.flat();
  const positive = values.filter((v) => v > 0);
  const minVal = positive.length ? Math.min(...positive) : 0;
  const maxVal = positive.length ? Math.max(...positive) : 1;
  const minLog = minVal > 0 ? Math.log10(minVal) : 0;
  const maxLog = maxVal > 0 ? Math.log10(maxVal) : 1;

  const canvas = document.createElement("canvas");
  canvas.width = width;
  canvas.height = height;
  const ctx = canvas.getContext("2d");
  if (!ctx) return null;
  const img = ctx.createImageData(width, height);

  for (let y = 0; y < height; y += 1) {
    for (let x = 0; x < width; x += 1) {
      const v = layer[y][x];
      const logv = v > 0 ? Math.log10(v) : minLog;
      const t = maxLog > minLog ? (logv - minLog) / (maxLog - minLog) : 0;
      const [r, g, b, a] = rampColor(t);
      const yy = height - 1 - y;
      const idx = (yy * width + x) * 4;
      img.data[idx] = r;
      img.data[idx + 1] = g;
      img.data[idx + 2] = b;
      img.data[idx + 3] = a;
    }
  }

  ctx.putImageData(img, 0, 0);
  return canvas.toDataURL("image/png");
}

function computeFluxStats(grid: FluxGrid): { min: number; max: number } | null {
  if (!grid.values.length) return null;
  const values = grid.values[0].flat().filter((v) => v > 0);
  if (!values.length) return { min: 0, max: 0 };
  return { min: Math.min(...values), max: Math.max(...values) };
}

function interpolateTrackPosition(track: TrackPoint[], now: Date): SmoothPos | null {
  if (track.length < 2) return null;
  const nowTs = now.getTime();
  for (let i = 0; i < track.length - 1; i += 1) {
    const a = track[i];
    const b = track[i + 1];
    const ta = new Date(a.t).getTime();
    const tb = new Date(b.t).getTime();
    if (nowTs < ta || nowTs > tb) continue;
    const span = tb - ta || 1;
    const t = Math.min(1, Math.max(0, (nowTs - ta) / span));
    return {
      t: now.toISOString(),
      lat: lerp(a.lat, b.lat, t),
      lon: lerpLon(a.lon, b.lon, t),
      alt_km: lerp(a.alt_km, b.alt_km, t),
    };
  }
  return null;
}

function rampColor(t: number): [number, number, number, number] {
  const clamped = Math.min(1, Math.max(0, t));
  const stops = [
    { t: 0.0, c: [10, 20, 60] },
    { t: 0.4, c: [0, 140, 200] },
    { t: 0.7, c: [255, 200, 60] },
    { t: 1.0, c: [240, 80, 30] },
  ];
  for (let i = 0; i < stops.length - 1; i += 1) {
    const a = stops[i];
    const b = stops[i + 1];
    if (clamped >= a.t && clamped <= b.t) {
      const span = b.t - a.t || 1;
      const tt = (clamped - a.t) / span;
      const r = Math.round(a.c[0] + (b.c[0] - a.c[0]) * tt);
      const g = Math.round(a.c[1] + (b.c[1] - a.c[1]) * tt);
      const bch = Math.round(a.c[2] + (b.c[2] - a.c[2]) * tt);
      return [r, g, bch, 180];
    }
  }
  return [0, 0, 0, 0];
}
