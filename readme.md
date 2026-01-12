# MASS-Cube

## 面向零基础的项目介绍
MASS-Cube 是一个端到端的航天任务演示系统。它把“卫星轨道 → 辐射通量 → 观测开关决策 → 3D 可视化”串起来，方便在网页上直观看到卫星位置、风险变化，以及系统为何决定观测开/关。

你可以把它理解为“会看轨道、会算风险、会告诉你什么时候该关机”的小型卫星任务演示平台。

## 系统做了什么
1) 获取 TLE（两行轨道根数）
2) 用 SGP4 推进轨道，得到任意时刻的卫星位置
3) 根据位置估计辐射通量
4) 把通量转换为风险分数，输出观测开/关窗口
5) 前端 3D 地球展示轨迹与风险，左侧面板可改参数并实时生效

## 设计思路（整体）
- 分层清晰：TLE、轨道、通量、决策分别是独立服务模块，便于替换数据源或算法。
- 配置驱动：所有阈值、权重、时间参数都放在 `config/config.yaml`，并支持 API 热更新。
- 可回滚：配置更新会保存版本历史，便于追溯与回退。
- 可靠兜底：没有 IRENE CLI 时自动回退到 mock 通量模型，保证系统可运行。
- 前后端分离：前端默认 `/api` 反向代理访问后端，适配本地与服务器部署。

## 后端设计（FastAPI）
**入口与路由**
- `app/main.py`：应用入口，提供健康检查、配置、轨道、通量、决策等 API。

**核心服务**
- `tle_service.py`：从官方 TLE 源抓取并评估质量，失败时降级。
- `orbit_service.py`：SGP4 轨道推进，TEME → ECEF → 经纬高转换。
- `flux_service.py`：通量模型封装（IRENE CLI 或 mock）。
- `decision_service.py`：把通量转换为风险，并输出观测开/关窗口。

**配置系统**
- `app/config.py`：读取 `config/config.yaml`，负责校验、版本记录与回滚。
- `config/config_versions.json`：记录历史配置版本。

## 前端设计（Vite + React + Cesium）
**目标**
- 左侧：实时状态与参数配置面板
- 右侧：3D 地球与轨迹可视化
- 改参数后，立即刷新轨迹/通量/决策结果

**数据流**
- 首次加载：`/config`、`/sat/track`、`/env/flux/track`、`/decision/windows`
- 实时更新：定时轮询状态与轨迹

## 选择的模型与策略
**轨道模型**
- 采用 Vallado SGP4（`sgp4` 包）进行轨道推进
- 坐标转换优先 `pymap3d.teme2ecef`

**通量模型**
- 默认 mock 通量：保证本地/服务器可运行
- 可选 AE9/AP9：通过 IRENE CLI 计算

**风险模型（决策核心）**
- 风险分数 = 加权通量的幂指数合成（参数可配置）
- 通过 `r_off` / `r_on`、`hold_time`、`lead_time` / `lag_time` 决定开关窗口

## 轨迹与时间长度（默认值）
这些参数来自 `config/config.yaml`，可在网页中修改并热更新：
- 轨道预测时长：4 小时（`orbits.prediction_horizon_sec`）
- 前端默认轨迹长度：2 小时（`ui.track_length_sec` = 7200 秒）
- 轨迹步长：60 秒（`orbits.track_step_sec`）

## 主要接口（示例）
- 健康检查：`GET /healthz`
- 配置读取/更新：`GET /config`、`PUT /config`
- 卫星状态：`GET /sat/state`
- 轨迹：`GET /sat/track?start&end&step`
- 通量：`GET /env/flux/track?start&end&step&percentile`
- 决策窗口：`GET /decision/windows?start&end&step`

## 如何运行
详细步骤见 `manual.md`。