# Orbit Quick

这个目录提供一个简洁的命令行工具，用于按卫星编号拉取 TLE，使用 SGP4 推进轨道并输出未来一天的经纬度轨迹。
输出文件格式与 `D:\grid\automatic_cmd\orbit\2026011912.txt` 类似，包含时间、纬度、经度、高度以及
AE8/AP8 计算得到的电子/质子通量两列（通过 IRBEM-LIB 计算）。

## 依赖
使用根目录 `requirements.txt` 中的依赖：`httpx`, `sgp4`, `pymap3d`, `numpy`, `python-dateutil`。

## 用法

```powershell
python orbit_cli.py --norad-id 65488 --hours 24 --step 10 --out .\output --official-url http://8.141.94.91/tle/ty47.txt
```

参数说明：
- `--norad-id`：NORAD 编号，必填
- `--start`：起始时间（ISO-8601），默认当前 UTC 时间
- `--hours`：预测时长（小时），默认 24
- `--step`：步长（秒），默认 10
- `--out`：输出文件路径或目录；如果是目录，会按 `YYYYMMDDHH.txt` 命名
- `--official-url`：官方源 TLE 地址（可选）
- `--official-user` / `--official-pass`：官方源认证（可选）
- `--celestrak-url`：CelesTrak 地址（默认 https://celestrak.org/NORAD/elements/gp.php）

## 输出格式
头两行是列名和分隔线，后续每行六个字段：
1. `Time (UTC)`（格式：`DD Mon YYYY HH:MM:SS.mmm`）
2. `Detic Latitude (deg)`
3. `Detic Longitude (deg)`
4. `Detic Altitude (km)`
5. `Ele Flux (0.650 MeV) (m^-2*sec^-1*MeV^-1)`
6. `Pro Flux (1.500 MeV) (m^-2*sec^-1*MeV^-1)`

## AE8/AP8 后端（IRBEM-LIB）
本目录默认使用 `orbit_quick/ae8ap8_irbem.py` 作为 AE8/AP8 计算后端。该后端不直接绑定 IRBEM-LIB，
而是调用外部命令（由你提供）完成计算。

### 约定的输入输出
后端会向外部命令传入一个 JSON 文件（`{input}`），内容格式：

```json
{
  "ele_energy_mev": 0.650,
  "pro_energy_mev": 1.500,
  "points": [
    {"mjd": 59580.0, "lat": 10.0, "lon": 20.0, "alt_km": 500.0}
  ]
}
```

外部命令需要输出 JSON 文件（`{output}`），内容格式：

```json
{
  "ele_flux_cm2": [123.0],
  "pro_flux_cm2": [456.0]
}
```

### 配置方式
设置环境变量 `IRBEM_CMD_TEMPLATE` 为调用外部命令的模板：

```powershell
$env:IRBEM_CMD_TEMPLATE = "python .\\irbem_cli.py --input {input} --output {output}"
```

Linux 示例：

```bash
export IRBEM_CMD_TEMPLATE="python ./irbem_cli.py --input {input} --output {output}"
```

如果不设置 `IRBEM_CMD_TEMPLATE`，脚本会默认尝试调用同目录下的 `irbem_cli.py`。

`irbem_cli.py` 支持 ESA 插值版本（低轨更平滑）：`--ele-model ae8min-esa` / `--pro-model ap8min-esa`。

如果你实现了其他后端模块，可通过环境变量切换：

```bash
export AE8AP8_BACKEND_MODULE=my_backend_module
```

该模块必须提供：

```python
def compute_ae8_ap8_flux(track, ele_energy_mev, pro_energy_mev) -> (np.ndarray, np.ndarray)
```
