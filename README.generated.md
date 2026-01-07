# MASS-Cube 后端实现（Python + FastAPI）

## 项目结构
- `app/main.py`：FastAPI 入口，提供健康检查、配置管理、轨道/通量/决策接口。
- `app/config.py`：统一配置模型，YAML 读写与版本化、热更新接口。
- `app/models.py`：Pydantic 数据模型（TLE、状态、通量、风险、窗口等）。
- `app/services/`：模块化服务
  - `tle_service.py`：官方 TLE 源（http://8.141.94.91/tle/ty47.txt）优先，CelesTrak 备源（含降级 mock）、质量评估、历史存储（SQLite）。
  - `orbit_service.py`：SGP4 传播，TEME→ECEF→经纬高（优先 pymap3d.teme2ecef，不支持时回退 eci2ecef/GMST）。
  - `flux_service.py`：可插拔通量模型（mock/AE9AP9 stub）。
  - `decision_service.py`：Risk 计算、阈值/迟滞/防抖、lead/lag 窗口。
- `config/config.yaml`：默认可运行配置，可通过 API 热更新；`config.example.yaml` 作为模版。
- `requirements.txt`：依赖列表。
- `tests/`：最小单测覆盖配置回滚与决策迟滞逻辑。

## 快速启动
```bash
python -m venv .venv
. .venv/Scripts/activate   # Windows
pip install -r requirements.txt
uvicorn app.main:app --reload --port 8000
```

## 关键 API
- `GET /healthz` 健康检查。
- `GET /config` / `PUT /config` 配置查看/热更新（含 schema 校验与版本号）。
- `GET /sat/state?time=...` 指定时刻卫星位置/速度/质量分。
- `GET /sat/track?start=...&end=...&step=...` 轨迹点串（默认长度/步长由配置驱动）。
- `GET /env/flux/track?...` 沿轨通量时序（mock 或 AE9/AP9 stub）。
- `GET /decision/windows?...` 观测 ON/OFF 窗口（含 lead/lag、迟滞、防抖、风险放大）。

## 配置与热更新
- 全部阈值/权重/迟滞时间均在 `config/config.yaml`，可通过 `PUT /config` 一次性提交整份配置。
- 更新后立即生效，写回 YAML，并追加版本日志 `config/config_versions.json`，可用 `ConfigStore.rollback(version)` 回滚。
- 环境变量暂不内置覆盖，保持文件 + API 一致性，便于外部配置中心接管。

## TLE 管线
- 默认尝试官方 TLE 源（http://8.141.94.91/tle/ty47.txt），失败时降级 CelesTrak，再失败则用内置 mock TLE。
- 质量评估：新鲜度、轨道要素跳变、传播偏差（均可配置权重/阈值），生成 `score` 和 `warnings`。
- 历史：SQLite `data/tle_history.db`，`list_recent` 自动按配置天数筛选；按时间/评分选择最优 TLE。

## 决策逻辑
- 风险：按配置的能段权重，取 `value**0.1` 的对数量级刻度合成 Risk。
- 迟滞/防抖：`R_off/R_on` + `hold_time` + `min_on/off_time`；`lead_time/lag_time` 形成缓冲窗口。
- 轨道质量放大：当 `orbit_quality < 0.5` 时乘以 `inflate_factor`。

## 测试
```bash
python -m pytest
```

## 后续对接（标记 TODO）
- 将 `flux.model` 切换为 AE9/AP9 IRENE 适配器时，替换 `FluxService._build_model` 中的 stub。
- 如需接入其他源，可在 `TLESourceConfig` 中新增或替换主源；完善传播偏差度量与 TLE 推演比对。
- 增强 config API：提供单版本回滚接口、schema 详情、差异预览。
