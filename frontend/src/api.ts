import type { AppConfig, ConfigResponse, FluxGrid, FluxSample, OrbitState, TrackPoint, Window } from "./types";

const API_BASE =
  import.meta.env.VITE_API_BASE ??
  (import.meta.env.DEV ? "http://127.0.0.1:8000" : "/api");

async function fetchJson<T>(path: string, init?: RequestInit): Promise<T> {
  const res = await fetch(`${API_BASE}${path}`, init);
  if (!res.ok) {
    const body = await res.text();
    throw new Error(`${res.status} ${res.statusText}: ${body}`);
  }
  return res.json();
}

export async function getConfig(): Promise<ConfigResponse> {
  return fetchJson("/config");
}

export async function updateConfig(config: AppConfig): Promise<ConfigResponse> {
  return fetchJson("/config", {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(config),
  });
}

export async function getState(timeIso?: string): Promise<OrbitState> {
  const q = timeIso ? `?time=${encodeURIComponent(timeIso)}` : "";
  return fetchJson(`/sat/state${q}`);
}

export async function getTrack(startIso: string, endIso: string, step?: number): Promise<TrackPoint[]> {
  const params = new URLSearchParams({ start: startIso, end: endIso });
  if (step) params.append("step", String(step));
  return fetchJson(`/sat/track?${params.toString()}`);
}

export async function getFlux(startIso: string, endIso: string, step: number, percentile: string): Promise<FluxSample[]> {
  const params = new URLSearchParams({
    start: startIso,
    end: endIso,
    step: String(step),
    percentile,
  });
  return fetchJson(`/env/flux/track?${params.toString()}`);
}

export async function getWindows(startIso: string, endIso: string, step: number, percentile: string): Promise<Window[]> {
  const params = new URLSearchParams({
    start: startIso,
    end: endIso,
    step: String(step),
    percentile,
  });
  return fetchJson(`/decision/windows?${params.toString()}`);
}

export async function exportObservationPlan(startIso: string, hours = 24): Promise<Blob> {
  const params = new URLSearchParams({
    start: startIso,
    hours: String(hours),
  });
  const res = await fetch(`${API_BASE}/decision/plan/export?${params.toString()}`);
  if (!res.ok) {
    const body = await res.text();
    throw new Error(`${res.status} ${res.statusText}: ${body}`);
  }
  return res.blob();
}

export async function getFluxGrid(
  timeIso: string,
  channel: string,
  percentile: string,
  altKm?: number
): Promise<FluxGrid> {
  const params = new URLSearchParams({
    time: timeIso,
    channel,
    percentile,
  });
  if (altKm !== undefined) {
    params.append("alt_km", String(altKm));
  }
  return fetchJson(`/env/flux/grid?${params.toString()}`);
}
