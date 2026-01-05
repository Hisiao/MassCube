# MASS-Cube 使用说明（面向零基础）

## 整体逻辑长话短说
- 框架：用 FastAPI 提供一组 HTTP 接口，便于浏览器或前端调用。
- 配置：启动时读取 `config/config.yaml`，变更会在内存里热加载，并写入版本日志 `config/config_versions.json`。
- TLE 获取：后台循环定期从 CelesTrak 拉取目标星（默认 NORAD 65490）的 TLE，失败则用内置 mock；同时把结果存入 SQLite `data/tle_history.db`。
- 质量评估：每条新 TLE 都按新鲜度、轨道要素跳变、传播偏差打分，生成 `score` 和 `warnings`。
- 轨道计算：`OrbitService` 用 SGP4 把 TLE 传播到指定时间，输出经纬度、高度、速度。
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
  - 通量序列：`/env/flux/track?start=...&end=...&step=60&percentile=mean`（mock 数据）
  - 决策窗口：`/decision/windows?start=...&end=...&step=60`
- 配置查看/修改：
  - GET `/config` 查看当前配置与版本。
  - PUT `/config` 提交完整 JSON（与 `config/config.yaml` 结构一致）即可热更新；保存的版本会写入 `config/config_versions.json`。

## 重要文件速览
- `app/main.py`：FastAPI 入口、路由与后台 TLE 刷新任务。
- `app/config.py`：配置模型、加载与版本管理。
- `app/services/`：核心业务（TLE 拉取与质量评估、轨道传播、通量生成、决策）。
- `app/models.py`：接口输入输出的数据结构。
- 数据与配置：`config/config.yaml`、`config/config_versions.json`、`data/tle_history.db`。

## 小贴士
- 如果接口返回 “No TLE available”，等后台刷新一轮或重启服务再试。
- 目前通量与风险为 mock 逻辑，用于流程验证；替换真模型时只需调整 `flux.model` 及对应适配。
