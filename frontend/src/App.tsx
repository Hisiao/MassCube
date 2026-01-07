import { useEffect, useMemo, useState } from "react";
import { Cartesian3, Color, Viewer as CesiumViewer } from "cesium";
import { Viewer, Entity } from "resium";
import {
  getConfig,
  getFlux,
  getState,
  getTrack,
  getWindows,
  updateConfig,
} from "./api";
import type {
  AppConfig,
  ConfigResponse,
  FluxSample,
  OrbitState,
  TrackPoint,
  Window,
} from "./types";

type RiskPoint = { t: string; risk: number };

const fmt = (v: number, digits = 3) => v.toFixed(digits);
const toIso = (d: Date) => d.toISOString();

function normalizeLon(lon: number) {
  return ((lon + 180) % 360) - 180;
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
  const [windows, setWindows] = useState<Window[]>([]);
  const [riskSeries, setRiskSeries] = useState<RiskPoint[]>([]);
  const [mode, setMode] = useState<{ mode: "OBS_ON" | "OBS_OFF"; reason?: string }>({ mode: "OBS_ON" });
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

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
      const risks = computeRiskSeries(flx, configResp.config.decision.risk_weights);
      setRiskSeries(risks);
      setMode(currentMode(win, now));
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

  const riskByTime = useMemo(() => {
    const map = new Map<string, number>();
    riskSeries.forEach((r) => map.set(r.t, r.risk));
    return map;
  }, [riskSeries]);

  const trackSegments = useMemo(() => {
    if (!track.length) return [];
    const segments: { positions: Cartesian3[]; color: Color }[] = [];
    let current: { positions: Cartesian3[]; color: Color } | null = null;

    for (const pt of track) {
      const risk = riskByTime.get(pt.t) ?? 0;
      const color = colorForRisk(risk);
      const pos = Cartesian3.fromDegrees(normalizeLon(pt.lon), pt.lat, pt.alt_km * 1000);
      if (!current || !current.color.equals(color)) {
        current = { positions: [pos], color };
        segments.push(current);
      } else {
        current.positions.push(pos);
      }
    }
    return segments;
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

        <div className="panel">
          <h3 title="未来 24h 观测窗口">观测窗口</h3>
          {windows.length ? (
            <div className="timeline">
              {windows.slice(0, 10).map((w) => (
                <div key={`${w.t_start}-${w.t_end}`} className={`chip ${w.mode === "OBS_OFF" ? "off" : ""}`}>
                  {w.mode} {new Date(w.t_start).toLocaleTimeString()} → {new Date(w.t_end).toLocaleTimeString()}
                </div>
              ))}
            </div>
          ) : (
            <div className="status">暂无窗口</div>
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
          {trackSegments.map((seg, idx) => (
            <Entity
              key={`seg-${idx}`}
              polyline={{
                positions: seg.positions,
                width: 3,
                material: seg.color,
              }}
            />
          ))}
          {state && (
            <Entity
              name="satellite"
              position={Cartesian3.fromDegrees(normalizeLon(state.lon), state.lat, state.alt_km * 1000)}
              point={{ pixelSize: 10, color: Color.CYAN, outlineColor: Color.WHITE, outlineWidth: 2 }}
              description={`Lat: ${fmt(state.lat)}, Lon: ${fmt(state.lon)}, Alt: ${fmt(state.alt_km)} km`}
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
      </section>
    </div>
  );
}

function computeRiskSeries(samples: FluxSample[], weights: Record<string, number>): RiskPoint[] {
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
      if (val > 0) risk += w * Math.pow(val, 0.1);
    }
    res.push({ t, risk });
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
