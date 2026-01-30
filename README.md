# MASS-Cube

## 1. 项目概述
MASS-Cube 是一个端到端的航天任务演示系统，串联“卫星轨道 → 辐射通量 → 观测开/关决策 → 3D 可视化”。后端负责 TLE 获取、轨道推进、通量估计与风险决策；前端以 3D 地球展示轨迹、风险与全局通量热力图，并提供参数热更新与观测计划导出。

项目的技术亮点包括：
- 服务分层清晰：TLE、轨道、通量、决策各自独立，便于替换数据源或模型。
- 配置驱动并可追溯：`config/config.yaml` 支持通过 API 更新并记录版本历史；回滚能力在 `ConfigStore` 中实现，但当前未暴露 API。
- 模型可降级：AE9/AP9 CLI 未配置时自动回退到 mock 通量模型。
- 结果可交付：支持导出未来观测计划 CSV（/decision/plan/export）。

## 2. 技术栈
- 前端
  - React + TypeScript
  - Vite 构建
  - Cesium + Resium 进行 3D 可视化
- 后端
  - FastAPI + Uvicorn
  - Pydantic 数据模型
  - httpx 异步请求
  - sgp4 轨道推进、pymap3d 坐标转换、numpy 数值计算
  - PyYAML 与 python-dateutil
- 数据与存储
  - SQLite（TLE 历史库：`data/tle_history.db`）
  - YAML 配置与 JSON 版本记录
  - AE9/AP9 计算缓存（`data/ae9ap9_cache`）
- 开发与部署
  - Docker / Docker Compose
  - Nginx 作为前端容器静态资源与反向代理
  - pytest（测试）

## 3. 项目架构
- 配置层：`ConfigStore` 读取/更新 `config/config.yaml`，写入 `config/config_versions.json`，更新后重建服务实例。
- 业务服务层：
  - `TLEService` 定期拉取 TLE 并进行质量评估，保存到 SQLite。
  - `OrbitService` 使用 SGP4 推进轨道并输出地理坐标与轨道质量。
- `FluxService` 基于轨道点计算通量序列或网格（mock、AE9/AP9 CLI 或 AP8/AE8 SPENVIS 网格）。
  - `DecisionService` 将通量转为风险并生成观测开/关窗口。
- API 层：`app/main.py` 提供状态、轨迹、通量、决策窗口与观测计划导出等接口，并在启动时启动 TLE 刷新与缓存清理循环。
- 前端层：`frontend/src/App.tsx` 定时轮询后端，生成风险序列与轨迹分段渲染，并叠加通量热力图。

## 4. 目录结构
已忽略 `.venv`、`dist`、`node_modules`、`.git` 等目录。

```
MASS-Cube/
  app/
    main.py                 # FastAPI 入口与路由
    config.py               # 配置模型、校验、版本记录与回滚
    models.py               # Pydantic 数据模型
    services/
      tle_service.py        # TLE 拉取、质量评估与 SQLite 存储
      orbit_service.py      # SGP4 推进与坐标转换
      flux_service.py       # 通量模型（mock / AE9AP9 CLI / AP8AE8 SPENVIS）
      decision_service.py   # 风险计算与开关窗口
    tools/
      irene_cli.py          # IRENE CLI 适配器
  config/
    config.yaml             # 默认配置
    config.docker.yaml      # Docker 默认配置（mock 通量）
    config.example.yaml     # 示例配置
    config_versions.json    # 配置版本历史
  data/
    tle_history.db          # TLE 历史数据库
    ae9ap9_cache/           # AE9/AP9 计算缓存
  frontend/
    src/
      App.tsx               # 主界面与数据流
      api.ts                # 后端 API 调用
      types.ts              # 前端类型定义
      main.tsx              # 前端入口
      index.css             # 样式
    index.html              # HTML 入口
    vite.config.ts          # Vite 配置
    nginx.conf              # Nginx 反向代理配置
    Dockerfile              # 前端构建与部署
  Irene/                     # IRENE 模型与资源（AE9/AP9）
  tests/                     # pytest 测试
  Dockerfile                 # 后端容器构建
  docker-compose.yml         # 一体化部署编排
  requirements.txt           # 后端依赖
  manual.md                  # 运行手册
  README.md                  # 项目说明
```

## 5. 核心文件说明
- 项目入口文件和配置文件
  - `app/main.py`：FastAPI 应用入口与路由定义，启动后台刷新与清理任务。
  - `app/config.py`：配置模型、校验、热更新与版本记录。
  - `config/config.yaml`：默认运行配置。
  - `config/config.docker.yaml`：Docker 环境默认配置（mock 通量）。
  - `docker-compose.yml` / `Dockerfile`：一体化部署入口与镜像构建。
- 核心业务逻辑实现
  - `app/services/tle_service.py`：TLE 抓取、质量评分与选择策略。
  - `app/services/orbit_service.py`：SGP4 推进与地理坐标转换。
  - `app/services/flux_service.py`：通量模型与网格/序列计算，含缓存逻辑。
  - `app/services/decision_service.py`：风险计算与观测窗口生成。
  - `app/tools/irene_cli.py`：将轨道点转换为 IRENE CLI 输入并解析输出。
- 数据模型和 API 接口
  - `app/models.py`：后端数据模型（轨道、通量、窗口、计划项等）。
  - `frontend/src/types.ts`：前端与后端接口契约。
  - `frontend/src/api.ts`：前端 API 封装与错误处理。
- 关键组件和服务模块
  - `frontend/src/App.tsx`：界面布局、数据拉取、风险计算与可视化逻辑。
  - `frontend/src/index.css`：主题与布局样式。
  - `frontend/vite.config.ts`：Cesium 资源路径配置与本地开发端口。
  - `frontend/nginx.conf`：生产环境前后端反向代理。

  生成或更新README.md时的具体要求：

准确性要求：
1. 必须实际阅读文件内容来了解功能，严禁基于文件名猜测
2. 对已存在的README内容进行全面校验，识别并更新过时信息
3. 使用工具验证目录结构的准确性，确保没有遗漏或错误的文件引用
4. 对现有文件重新阅读和分析，因为内容可能已经更新

内容要求：
5. 使用纯文本（markdown）格式，避免emoji和装饰性符号
6. 内容要既准确又简洁，避免冗余描述
7. 面向不同技术水平的读者，提供必要的背景说明
8. 重点突出项目的独特设计和技术亮点
9. 忽略.venv、dist、node_modules、.git等目录

结构要求：
10. 保持逻辑清晰的层次结构
11. 确保各部分内容的关联性和完整性
