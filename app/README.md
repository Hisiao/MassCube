# app 后端说明

## 1. 项目概述
`app` 目录是 MASS-Cube 的后端实现，负责 TLE 获取与质量评估、轨道推进、通量计算、风险决策以及 API 服务。系统支持配置热更新并保留版本历史（回滚能力在 `ConfigStore` 内实现，但当前未暴露 API），并在启动后运行后台任务（TLE 刷新、缓存清理）。

## 2. 技术栈
- 前端
  - 无（由 `frontend/` 目录提供）
- 后端
  - FastAPI + Uvicorn
  - Pydantic 数据模型与校验
  - httpx 异步请求
  - sgp4 轨道推进、pymap3d 坐标转换、numpy 数值计算
  - PyYAML 与 python-dateutil
- 数据与存储
  - SQLite（`data/tle_history.db`）存储 TLE 历史
  - YAML 配置与 JSON 版本历史
  - AE9/AP9 通量缓存（`data/ae9ap9_cache`）
- 开发工具
  - pytest（测试）

## 3. 项目架构
- 配置层：`ConfigStore` 从 `config/config.yaml` 加载配置，更新后写入 `config/config_versions.json`，并触发服务重建。
- 服务层：
  - `TLEService` 从 `TLESourceConfig.primary_url` 拉取 TLE，失败时回退到 CelesTrak；如仍失败则使用 mock TLE，并进行质量评分后保存到 SQLite。
  - `OrbitService` 使用 SGP4 推进轨道并输出经纬高。
  - `FluxService` 根据轨道点计算通量序列或网格，支持 mock、AE9/AP9 CLI 与 AP8/AE8 SPENVIS 网格。
  - `DecisionService` 将通量映射到风险并生成观测开/关窗口。
- API 层：`app/main.py` 提供配置、轨道、通量、决策与计划导出接口，并在启动时创建后台任务（TLE 刷新、缓存清理）。

## 4. 目录结构
```
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
  __init__.py
```

## 5. 核心文件说明
- 项目入口文件和配置文件
  - `app/main.py`：API 定义、启动/关闭任务、数据流编排。
  - `app/config.py`：配置结构、校验规则、版本管理与回滚。
- 核心业务逻辑实现
  - `app/services/tle_service.py`：TLE 拉取、质量评分与择优策略。
  - `app/services/orbit_service.py`：轨道推进与 TEME→ECEF→地理坐标转换。
  - `app/services/flux_service.py`：通量序列/网格计算与缓存。
  - `app/services/decision_service.py`：风险计算与观测窗口决策。
  - `app/tools/irene_cli.py`：将轨道点转换为 IRENE CLI 输入并解析输出。
- 数据模型和 API 接口
  - `app/models.py`：轨道、通量、风险、窗口、计划项等模型。
- 关键组件和服务模块
  - `app/services/*`：核心业务模块入口。

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
