from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from typing import Iterable

import numpy as np


def _datetime_to_mjd(dt: datetime) -> float:
    jd = (dt - datetime(2000, 1, 1, tzinfo=timezone.utc)).total_seconds() / 86400.0 + 2451544.5
    return jd - 2400000.5


def compute_ae8_ap8_flux(
    track: Iterable[tuple[datetime, float, float, float]],
    ele_energy_mev: float,
    pro_energy_mev: float,
) -> tuple[np.ndarray, np.ndarray]:
    cmd_template = os.getenv("IRBEM_CMD_TEMPLATE")
    if not cmd_template:
        local_cli = Path(__file__).with_name("irbem_cli.py")
        if local_cli.exists():
            cmd_template = f"\"{sys.executable}\" \"{local_cli}\" --input {{input}} --output {{output}}"
        else:
            raise RuntimeError(
                "IRBEM_CMD_TEMPLATE not set. Provide a command that reads {input} JSON and writes {output} JSON."
            )

    points = []
    for t, lat, lon, alt in track:
        points.append(
            {
                "mjd": _datetime_to_mjd(t),
                "lat": float(lat),
                "lon": float(lon),
                "alt_km": float(alt),
            }
        )

    payload = {
        "ele_energy_mev": float(ele_energy_mev),
        "pro_energy_mev": float(pro_energy_mev),
        "points": points,
    }

    with tempfile.TemporaryDirectory() as tmpdir:
        input_path = Path(tmpdir) / "irbem_input.json"
        output_path = Path(tmpdir) / "irbem_output.json"
        input_path.write_text(json.dumps(payload), encoding="utf-8")
        cmd = cmd_template.format(input=str(input_path), output=str(output_path))
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        if result.returncode != 0:
            raise RuntimeError(f"IRBEM command failed: {result.stderr.strip()}")
        if not output_path.exists():
            raise RuntimeError("IRBEM command did not produce output JSON.")
        data = json.loads(output_path.read_text(encoding="utf-8"))

    ele_flux_cm2 = data.get("ele_flux_cm2")
    pro_flux_cm2 = data.get("pro_flux_cm2")
    if ele_flux_cm2 is None or pro_flux_cm2 is None:
        raise RuntimeError("IRBEM output missing ele_flux_cm2 or pro_flux_cm2 arrays.")

    ele_flux_cm2 = np.asarray(ele_flux_cm2, dtype=np.float64)
    pro_flux_cm2 = np.asarray(pro_flux_cm2, dtype=np.float64)
    if ele_flux_cm2.shape[0] != len(points) or pro_flux_cm2.shape[0] != len(points):
        raise RuntimeError("IRBEM output length mismatch.")

    return ele_flux_cm2 * 1.0e4, pro_flux_cm2 * 1.0e4
