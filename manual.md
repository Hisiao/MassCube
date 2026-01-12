# MASS-Cube 操作说明

## 总览
- 后端：FastAPI 提供配置、轨道、通量、决策接口。
- 前端：Vite + React + Cesium，可视化 3D 地球与实时参数。
- 默认配置：`config/config.yaml`（本地）与 `config/config.docker.yaml`（Docker）。

## 本地运行
1) 安装后端依赖
```bash
python -m venv .venv
. .venv/Scripts/activate   # Windows
pip install -r requirements.txt
```
2) 启动后端
```bash
uvicorn app.main:app --reload --port 8000
```
3) 启动前端
```bash
cd frontend
cp .env.example .env
npm install
npm run dev
```
4) 访问
- 前端：`http://127.0.0.1:5173`
- 后端接口文档：`http://127.0.0.1:8000/docs`

## Ubuntu 服务器部署（Docker）
前提：服务器已安装 Docker 29.1.3+ 与 Docker Compose。

1) 在服务器上克隆或上传代码
```bash
git clone <your-repo>
cd MASS-Cube
```
2) 启动服务
```bash
docker compose up -d --build
```
3) 开放端口
- 前端：80
- 后端：8000（如需访问 `/docs`）

4) 访问
- 前端：`http://<server-ip>/`
- 后端：`http://<server-ip>:8000/docs`

## Docker 配置说明
- Docker 默认使用 `config/config.docker.yaml`，已切换为 mock 通量，确保在 Ubuntu 上可运行。
- 通过 `PUT /config` 更新的配置会写回 `config/config.docker.yaml` 与 `config/config_versions.json`。
- 若需 AE9/AP9（IRENE CLI）：
  1) 提供 Linux 可执行版 IRENE CLI（Windows 的 `.exe` 无法在 Ubuntu 直接运行）。
  2) 修改 `config/config.docker.yaml` 中 `flux.model`、`flux.ae9ap9.executable` 和 `flux.ae9ap9.command_template`。
  3) 将 IRENE 相关目录挂载到容器并更新路径。

## 常用接口
- 健康检查：`GET /healthz`
- 配置读取/更新：`GET /config`、`PUT /config`
- 轨道状态：`GET /sat/state`
- 轨道轨迹：`GET /sat/track?start&end&step`
- 通量序列：`GET /env/flux/track?start&end&step&percentile`
- 通量网格：`GET /env/flux/grid?time&channel&percentile&alt_km`
- 决策窗口：`GET /decision/windows?start&end&step`

## 常见问题
- 如果提示 “No TLE available”，等待后台刷新或重启服务再试。
- Docker 环境默认 mock 通量，如需 AE9/AP9 请按上文配置。
