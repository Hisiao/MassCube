from __future__ import annotations

import argparse
import json
import os
import subprocess
import tempfile
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, Iterable, List, Tuple


@dataclass(frozen=True)
class ChannelSpec:
    name: str
    model_type: str
    energy_mev: float


def _parse_channel(ch: str) -> ChannelSpec:
    # Expected formats like "Je>100keV", "Je>1MeV", "Jp>10MeV"
    if not ch.startswith(("Je>", "Jp>")):
        raise ValueError(f"unsupported channel format: {ch}")
    model_type = "AE9" if ch.startswith("Je>") else "AP9"
    suffix = ch.split(">")[1]
    if suffix.lower().endswith("kev"):
        val = float(suffix[:-3])
        energy_mev = val / 1000.0
    elif suffix.lower().endswith("mev"):
        val = float(suffix[:-3])
        energy_mev = val
    else:
        raise ValueError(f"unsupported energy unit in channel: {ch}")
    return ChannelSpec(name=ch, model_type=model_type, energy_mev=energy_mev)


def _to_ydd_gmt(t: str) -> Tuple[int, int, float]:
    dt = datetime.fromisoformat(t.replace("Z", "+00:00")).astimezone(timezone.utc)
    year = dt.year
    day = dt.timetuple().tm_yday
    gmt_sec = dt.hour * 3600 + dt.minute * 60 + dt.second + dt.microsecond / 1e6
    return year, day, gmt_sec


def _resolve_irene_home(arg_home: str | None) -> Path:
    if arg_home:
        return Path(arg_home).resolve()
    env_home = os.environ.get("IRENE_HOME")
    if env_home:
        return Path(env_home).resolve()
    repo_root = Path(__file__).resolve().parents[2]
    return (repo_root / "Irene").resolve()


def _irene_bin(irene_home: Path) -> Path:
    return irene_home / "win64" / "bin"


def _model_db(irene_home: Path, model_type: str) -> Path:
    if model_type == "AE9":
        name = "AE9V15_runtime_tables.mat"
    else:
        name = "AP9V15_runtime_tables.mat"
    return irene_home / "modelData" / name


def _support_db(irene_home: Path) -> Dict[str, Path]:
    base = irene_home / "modelData"
    return {
        "MagfieldDB": base / "igrfDB.h5",
        "KPhiDB": base / "fastPhi_net.mat",
        "KHminDB": base / "fast_hmin_net.mat",
    }


def _format_energy_list(energies: Iterable[float]) -> str:
    return ", ".join(f"{e:g}" for e in energies)


def _percentile_value(percentile: str) -> int | None:
    if percentile.lower() == "mean":
        return None
    if percentile.lower().startswith("p"):
        return int(percentile[1:])
    return int(percentile)


def _output_path(prefix: Path, percentile: str) -> Path:
    pct = _percentile_value(percentile)
    if pct is None:
        return prefix.with_name(f"{prefix.stem}_mean_flux.txt")
    return prefix.with_name(f"{prefix.stem}_pctile_flux_{pct}.txt")


def _write_input_file(
    path: Path,
    model_type: str,
    model_db: Path,
    support_db: Dict[str, Path],
    out_prefix: Path,
    orbit_file: Path,
    energies_mev: List[float],
    percentile: str,
) -> None:
    pct = _percentile_value(percentile)
    lines = [
        f"ModelType:   {model_type}",
        f"ModelDB:     {model_db}",
        f"OutFile:     {out_prefix}",
        f"{'MagfieldDB'}:  {support_db['MagfieldDB']}",
        f"{'KPhiDB'}:  {support_db['KPhiDB']}",
        f"{'KHminDB'}: {support_db['KHminDB']}",
        "FluxType:    1PtDiff",
        f"Energies:    {_format_energy_list(energies_mev)}",
    ]
    if pct is None:
        lines.append("FluxOut:     mean")
    else:
        lines.append(f"FluxOut:     percentile,{pct}")
    lines.extend(
        [
            f"OrbitFile:   {orbit_file}",
            "InTimeSpec:  YrDDDGmt",
            "InCoordSys:  GDZ",
            "InCoordUnits: km",
            "InCoordOrder: Inverted",
            "InDataDelim: comma",
        ]
    )
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def _write_orbit_file(path: Path, points: List[Dict]) -> None:
    lines = []
    for p in points:
        year, day, gmt = _to_ydd_gmt(p["time"])
        lat = float(p["lat"])
        lon = float(p["lon"])
        alt = float(p["alt_km"])
        lines.append(f"{year},{day},{gmt:.3f},{lat:.6f},{lon:.6f},{alt:.3f}")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def _parse_output(path: Path, energies_mev: List[float]) -> List[List[float]]:
    if not path.exists():
        raise RuntimeError(f"IRENE output not found: {path}")
    energy_line = None
    rows: List[List[float]] = []
    for raw in path.read_text(encoding="utf-8").splitlines():
        if raw.startswith("# Energy levels"):
            energy_line = raw
            continue
        if not raw or raw.startswith("#"):
            continue
        parts = [p.strip() for p in raw.split(",")]
        if len(parts) < 5:
            continue
        values = [float(x) for x in parts[4:]]
        rows.append(values)
    if energy_line is None:
        raise RuntimeError("IRENE output missing energy header")
    header_vals = energy_line.split(":")[1].strip().split()
    header_energies = [float(v) for v in header_vals]
    if len(header_energies) != len(energies_mev):
        raise RuntimeError("IRENE output energy count mismatch")
    return rows


def _index_map(energies_mev: List[float]) -> Dict[str, int]:
    idx = {}
    for i, e in enumerate(energies_mev):
        key = f"{e:.6g}"
        idx[key] = i
    return idx


def _run_irene(
    irene_home: Path,
    model_type: str,
    channels: List[ChannelSpec],
    points: List[Dict],
    percentile: str,
) -> Dict[str, List[float]]:
    energies_mev = [c.energy_mev for c in channels]
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        orbit_path = tmp / "orbit.csv"
        input_path = tmp / "input.txt"
        out_prefix = tmp / "irene_output.txt"
        _write_orbit_file(orbit_path, points)
        _write_input_file(
            input_path,
            model_type,
            _model_db(irene_home, model_type),
            _support_db(irene_home),
            out_prefix,
            orbit_path,
            energies_mev,
            percentile,
        )
        exe = _irene_bin(irene_home) / "CmdLineIrene.exe"
        cmd = f"\"{exe}\" -i \"{input_path}\""
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=_irene_bin(irene_home))
        if result.returncode != 0:
            raise RuntimeError(f"CmdLineIrene failed: {result.stderr.strip()}")
        output_path = _output_path(out_prefix, percentile)
        rows = _parse_output(output_path, energies_mev)
    if len(rows) != len(points):
        raise RuntimeError("IRENE output row count mismatch")
    energy_idx = _index_map(energies_mev)
    channel_values: Dict[str, List[float]] = {}
    for ch in channels:
        key = f"{ch.energy_mev:.6g}"
        idx = energy_idx.get(key)
        if idx is None:
            raise RuntimeError(f"energy {ch.energy_mev} MeV not found in output")
        channel_values[ch.name] = [row[idx] if idx < len(row) else 0.0 for row in rows]
    return channel_values


def main() -> int:
    parser = argparse.ArgumentParser(description="IRENE CLI adapter for MASS-Cube")
    parser.add_argument("--input", required=True, help="input JSON payload")
    parser.add_argument("--output", required=True, help="output JSON path")
    parser.add_argument("--irene-home", default="", help="path to Irene root directory")
    args = parser.parse_args()

    payload = json.loads(Path(args.input).read_text(encoding="utf-8"))
    channels = payload.get("channels", [])
    points = payload.get("points", [])
    percentile = payload.get("percentile", "mean")
    if not channels or not points:
        raise RuntimeError("channels and points are required")

    specs = [_parse_channel(ch) for ch in channels]
    irene_home = _resolve_irene_home(args.irene_home or None)

    grouped: Dict[str, List[ChannelSpec]] = {}
    for spec in specs:
        grouped.setdefault(spec.model_type, []).append(spec)

    values_by_channel: Dict[str, List[float]] = {}
    for model_type, model_channels in grouped.items():
        values_by_channel.update(_run_irene(irene_home, model_type, model_channels, points, percentile))

    out_points = []
    for idx in range(len(points)):
        vals = {ch: values_by_channel[ch][idx] for ch in channels}
        out_points.append({"values": vals})

    Path(args.output).write_text(json.dumps({"points": out_points}), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
