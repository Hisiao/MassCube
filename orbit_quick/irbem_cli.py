from __future__ import annotations

import argparse
import json
import math
import os
import shutil
import sys
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import Iterable

import ctypes
import numpy as np


MJD_EPOCH = datetime(1858, 11, 17, tzinfo=timezone.utc)
NENE_MAX = 25


def _mjd_to_datetime(mjd: float) -> datetime:
    return MJD_EPOCH + timedelta(days=float(mjd))


def _load_irbem_dll() -> ctypes.WinDLL:
    env_path = os.getenv("IRBEM_LIB_DLL")
    candidates: list[Path] = []
    if env_path:
        candidates.append(Path(env_path))
    here = Path(__file__).resolve().parent
    candidates.extend(
        [
            here / "IRBEM" / "libirbem.dll",
            here / "IRBEM" / "bin" / "libirbem.win64.gfortran64.dll",
        ]
    )
    dll_path = next((p for p in candidates if p.exists()), None)
    if dll_path is None:
        raise FileNotFoundError(
            "libirbem.dll not found. Set IRBEM_LIB_DLL or place it under orbit_quick/IRBEM."
        )

    gfortran = shutil.which("gfortran.exe")
    if gfortran:
        os.add_dll_directory(str(Path(gfortran).parent))

    return ctypes.WinDLL(str(dll_path))


def _get_ntime_max(lib: ctypes.WinDLL) -> int:
    func = getattr(lib, "get_irbem_ntime_max1_", None)
    if func is None:
        raise RuntimeError("IRBEM DLL missing get_irbem_ntime_max1_ symbol.")
    func.argtypes = [ctypes.POINTER(ctypes.c_int)]
    func.restype = None
    ntime = ctypes.c_int(-1)
    func(ctypes.byref(ntime))
    if ntime.value <= 0:
        raise RuntimeError("IRBEM returned invalid NTIME_MAX.")
    return int(ntime.value)


def _resolve_fly_in_nasa_aeap(lib: ctypes.WinDLL):
    for name in ("fly_in_nasa_aeap1_", "fly_in_nasa_aeap_"):
        func = getattr(lib, name, None)
        if func is not None:
            func.argtypes = [
                ctypes.POINTER(ctypes.c_int),  # ntime
                ctypes.POINTER(ctypes.c_int),  # sysaxes
                ctypes.POINTER(ctypes.c_int),  # whichm
                ctypes.POINTER(ctypes.c_int),  # whatf
                ctypes.POINTER(ctypes.c_int),  # nene
                ctypes.POINTER(ctypes.c_double),  # energy (2 x NENE_MAX)
                ctypes.POINTER(ctypes.c_int),  # iyear
                ctypes.POINTER(ctypes.c_int),  # idoy
                ctypes.POINTER(ctypes.c_double),  # ut
                ctypes.POINTER(ctypes.c_double),  # x1
                ctypes.POINTER(ctypes.c_double),  # x2
                ctypes.POINTER(ctypes.c_double),  # x3
                ctypes.POINTER(ctypes.c_double),  # flux (NTIME_MAX x NENE_MAX)
            ]
            func.restype = None
            return func
    raise RuntimeError("IRBEM DLL missing fly_in_nasa_aeap1_ symbol.")


def _energy_matrix(energy_mev: float) -> ctypes.Array:
    arr = (ctypes.c_double * (2 * NENE_MAX))()
    arr[0] = float(energy_mev)
    arr[1] = math.nan
    return arr


def _fill_time_arrays(
    points: Iterable[dict],
    ntime_max: int,
) -> tuple[ctypes.Array, ctypes.Array, ctypes.Array, ctypes.Array, ctypes.Array, ctypes.Array, int]:
    iyear = (ctypes.c_int * ntime_max)()
    idoy = (ctypes.c_int * ntime_max)()
    ut = (ctypes.c_double * ntime_max)()
    x1 = (ctypes.c_double * ntime_max)()
    x2 = (ctypes.c_double * ntime_max)()
    x3 = (ctypes.c_double * ntime_max)()

    count = 0
    for idx, pt in enumerate(points):
        dt = _mjd_to_datetime(pt["mjd"])
        iyear[idx] = dt.year
        idoy[idx] = dt.timetuple().tm_yday
        ut[idx] = dt.hour * 3600 + dt.minute * 60 + dt.second + dt.microsecond / 1.0e6
        x1[idx] = float(pt["alt_km"])
        x2[idx] = float(pt["lat"])
        lon = float(pt["lon"]) % 360.0
        if lon < 0:
            lon += 360.0
        x3[idx] = lon
        count += 1

    return iyear, idoy, ut, x1, x2, x3, count


def _compute_flux(
    lib: ctypes.WinDLL,
    points: list[dict],
    energy_mev: float,
    whichm: int,
    sysaxes: int = 0,
) -> np.ndarray:
    if not points:
        return np.array([], dtype=np.float64)

    ntime_max = _get_ntime_max(lib)
    func = _resolve_fly_in_nasa_aeap(lib)

    out = np.empty(len(points), dtype=np.float64)
    nene = ctypes.c_int(1)
    whatf = ctypes.c_int(1)
    sysaxes_c = ctypes.c_int(int(sysaxes))
    whichm_c = ctypes.c_int(int(whichm))
    energy = _energy_matrix(energy_mev)

    for start in range(0, len(points), ntime_max):
        chunk = points[start : start + ntime_max]
        iyear, idoy, ut, x1, x2, x3, ntime = _fill_time_arrays(chunk, ntime_max)
        flux = (ctypes.c_double * (ntime_max * NENE_MAX))()
        ntime_c = ctypes.c_int(ntime)

        func(
            ctypes.byref(ntime_c),
            ctypes.byref(sysaxes_c),
            ctypes.byref(whichm_c),
            ctypes.byref(whatf),
            ctypes.byref(nene),
            energy,
            iyear,
            idoy,
            ut,
            x1,
            x2,
            x3,
            flux,
        )

        for i in range(ntime):
            val = flux[i]
            if val < -1.0e30:
                val = 0.0
            out[start + i] = val

    return out


def _parse_model(name: str) -> int:
    key = name.strip().lower().replace("-", "").replace(" ", "")
    lookup = {
        "ae8min": 1,
        "ae8max": 2,
        "ap8min": 3,
        "ap8max": 4,
        "ae8minesa": -1,
        "ae8maxesa": -2,
        "ap8minesa": -3,
        "ap8maxesa": -4,
    }
    if key not in lookup:
        raise ValueError(f"Unknown model '{name}'. Use ae8min/ae8max/ap8min/ap8max.")
    return lookup[key]


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="IRBEM AE8/AP8 JSON bridge.")
    parser.add_argument("--input", required=True, help="Input JSON path")
    parser.add_argument("--output", required=True, help="Output JSON path")
    parser.add_argument("--ele-model", default="ae8min", help="Electron model (ae8min/ae8max)")
    parser.add_argument("--pro-model", default="ap8min", help="Proton model (ap8min/ap8max)")
    args = parser.parse_args(argv)

    input_path = Path(args.input)
    output_path = Path(args.output)

    payload = json.loads(input_path.read_text(encoding="utf-8"))
    points = payload.get("points", [])
    if not points:
        raise RuntimeError("Input JSON has no points.")

    ele_energy = float(payload["ele_energy_mev"])
    pro_energy = float(payload["pro_energy_mev"])

    lib = _load_irbem_dll()
    ele_flux = _compute_flux(lib, points, ele_energy, _parse_model(args.ele_model))
    pro_flux = _compute_flux(lib, points, pro_energy, _parse_model(args.pro_model))

    output = {
        "ele_flux_cm2": ele_flux.tolist(),
        "pro_flux_cm2": pro_flux.tolist(),
    }
    output_path.write_text(json.dumps(output), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
