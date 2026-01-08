# MASS-Cube 使用说明（面向零基础）

## 整体逻辑长话短说
- 框架：用 FastAPI 提供一组 HTTP 接口，便于浏览器或前端调用。
- 配置：启动时读取 `config/config.yaml`，变更会在内存里热加载，并写入版本日志 `config/config_versions.json`。
- TLE 获取：后台循环优先从官方 TLE 源（http://8.141.94.91/tle/ty47.txt）拉取目标星（默认 NORAD 65488）的 TLE，失败则降级 CelesTrak，仍失败才用内置 mock；同时把结果存入 SQLite `data/tle_history.db`。
- 质量评估：每条新 TLE 都按新鲜度、轨道要素跳变、传播偏差打分，生成 `score` 和 `warnings`。
- 轨道计算：`OrbitService` 用 SGP4 把 TLE 传播到指定时间，优先使用 `pymap3d.teme2ecef`（若不可用则用 `eci2ecef` 或 GMST 退化实现），再转换经纬度、高度、速度。
- 时间基准：所有轨道计算均按 UTC 时间；与外部网站对比需统一时间基准。
- 辐射通量：当前用 mock 模型，按高度生成各能道的粒子通量时间序列。
- 决策窗口：把通量映射为风险值，结合阈值、提前/滞后时间和最短保持时间，给出观测 ON/OFF 窗口。

## 怎么跑起来
1) 安装依赖（仅需一次）：
```
python -m venv .venv
. .venv/Scripts/activate   # Windows
pip install -r requirements.txt
```
2) 启动后端（开发热重载）：
```
uvicorn app.main:app --reload --port 8000
```
3) 控制台看到 “Application startup complete.” 表示成功；后台会按配置周期自动刷新 TLE。

## 网页端/浏览器操作
- 交互文档：打开 `http://127.0.0.1:8000/docs`，在页面上可视化调试所有接口（无需写代码）。
- 健康检查：浏览器输入 `http://127.0.0.1:8000/healthz` 应返回 `{"status":"ok","version":"0.1.0"}`。
- 常用查询示例（在 `/docs` 里点 “Try it out” 或直接地址栏输入）：
  - 当前状态：`/sat/state` 或指定时间 `?time=2026-01-05T15:50:00Z`
  - 轨道轨迹：`/sat/track?start=2026-01-05T15:50:00Z&end=2026-01-05T16:10:00Z&step=60`
  - 通量序列：`/env/flux/track?start=...&end=...&step=60&percentile=mean`（AE9/AP9 IRENE 输出）
  - 全球网格：`/env/flux/grid?time=...&channel=Je>1MeV&percentile=mean&alt_km=600`（用于热力图纹理，默认 2D 单高度层）
  - 决策窗口：`/decision/windows?start=...&end=...&step=60`
- 配置查看/修改：
  - GET `/config` 查看当前配置与版本。
  - PUT `/config` 提交完整 JSON（与 `config/config.yaml` 结构一致）即可热更新；保存的版本会写入 `config/config_versions.json`。

## 前端（Vite + React + Cesium）使用
1) 安装 Node.js 18+，进入 `frontend/` 安装依赖：
```
cd frontend
npm install
```
2) 开发/预览：
```
npm run dev    # 默认 http://127.0.0.1:5173
```
如需指向远程后端，可在 `frontend/.env` 中设置 `VITE_API_BASE=http://<backend>:8000`（默认本机）。
3) 页面布局与交互：
- 左侧：实时状态面板 + 参数配置（与 `/config` 对齐），可修改阈值/权重/分位等并“保存并应用”（PUT /config）。当前版本号和刷新状态显示在底部。
- 观测窗口：按时间列出 OBS_ON/OFF 段（来自 `/decision/windows`）。
- 右侧：Cesium 3D 地球，蓝绿橙轨迹按风险渐变，白色点为当前卫星位置，悬停显示坐标/高度。
- 数据刷新：默认每 15 秒自动轮询 state/track/flux/windows；保存配置后会立即重新拉取。

## 重要文件速览
- `app/main.py`：FastAPI 入口、路由与后台 TLE 刷新任务。
- `app/config.py`：配置模型、加载与版本管理。
- `app/services/`：核心业务（TLE 拉取与质量评估、轨道传播、通量生成、决策）。
- `app/models.py`：接口输入输出的数据结构。
- 数据与配置：`config/config.yaml`、`config/config_versions.json`、`data/tle_history.db`。
## 观测计划生成依据（/decision/plan/export）
- 计划时间范围：以输入 `start`（UTC）为起点，默认向后 24h，返回 OBS_ON 段计划。
- 轨道与通量：后端依据 `track_step_sec` 生成轨迹，对每个时间点计算多能道通量。
- 风险计算：对每个时间点使用加权幂指数模型：
  - `risk = risk_scale * Σ( weight[channel] * flux[channel]^risk_exponent )`，参数来自 `config.decision`。
  - 若 `orbit_quality < 0.5`，风险按 `inflate_factor` 放大，降低轨道质量带来的漏判风险。
- 开关机规则：
  - 当 `risk > r_off` 切到 OBS_OFF（提前 `lead_time_sec` 关机）。
  - 当 `risk < r_on` 切到 OBS_ON（延后 `lag_time_sec` 开机）。
  - `hold_time_sec/min_on_time_sec/min_off_time_sec` 约束最短保持时长。
- 导出内容：CSV 仅包含 OBS_ON 段（开机/关机/时长），并附 `norad_id`、`tle_epoch_utc`、`percentile`、`flux_model`、`track_step_sec` 作为追溯信息。

## 小贴士
- 如果接口返回 “No TLE available”，等后台刷新一轮或重启服务再试。
- 通量由 AE9/AP9 IRENE CLI 计算；如需使用 mock，可把 `flux.model` 改为 `mock`。
- IRENE CLI 输入输出约定：输入 JSON（包含 points/channels/percentile），输出 JSON（points[i].values）或 CSV（index,channel,value）。
- 若 CLI 未配置，系统会自动回退到 mock 通量模型，可在 `/config` 响应中查看 `flux_model`。
