你是一名资深航天任务软件工程师 + 全栈工程师。请生成一个端到端可运行的工程项目：后端负责从官方 TLE 源（http://8.141.94.91/tle/ty47.txt）获取 TLE（必要时降级 CelesTrak）、SGP4 传播轨道、计算电子/质子通量风险并生成观测规避/关机窗口；前端提供一个真实可用网页：左侧为参数配置与实时坐标面板，右侧为 3D 地球视图，用户能直观看到卫星位置/轨迹，并能调参让决策与可视化即时变化。

0. 背景与目标

卫星：NORAD ID = 65488，COSPAR = 2025-197C

轨道：官方 TLE 源（http://8.141.94.91/tle/ty47.txt）/CelesTrak → SGP4 传播 → 实时位置/轨迹（含未来预测）

环境：电子/质子通量用于保障 0.1–10 MeV 伽马谱线观测；进入高通量区（范艾伦辐射带相关高风险）时控制载荷关机

关键要求：阈值、权重、滞回、提前/延后时间、触发器、裕度放大因子全部配置化，并可通过网页修改，后端热更新立即生效（含版本号与回滚）。

1) 交付物（必须全部提供）
1.1 项目结构

输出一个完整 repo 结构（多文件），包含：

backend/（Python + FastAPI）

frontend/（Vite + React + TypeScript）

docker-compose.yml（建议） + .env.example

README.md（从零启动：一键运行后访问网页）

1.2 后端（FastAPI）

必须实现模块化服务（同进程也可，但代码结构要分层清晰）：

1) tle_service

主源：官方 TLE 源（http://8.141.94.91/tle/ty47.txt）

备源：CelesTrak（主源失败自动降级）

拉取：默认 900 秒（可配置），当前未提供手动触发 endpoint

保存：最近 14 天历史

TLE 质量门控与评分（必须实现并对外暴露）：

行校验和/解析完整性

新鲜度 now-epoch 阈值

要素变化阈值（与上一条相比）

传播偏差阈值（旧 TLE 外推到新 epoch 与新 TLE 差异）

输出标准化对象 TLESet（source, fetched_at, epoch, score, warnings[]）

2) orbit_service

使用 Vallado SGP4（python-sgp4）

必须实现“按时间选最优 TLE”：对任意时刻 t，选 epoch 最近且评分最高的 TLE

坐标转换：TEME → ECEF → LLA（经纬高），优先使用 pymap3d.teme2ecef + ecef2geodetic；若库不支持则回退到 eci2ecef/GMST（无外部 EOP 数据）

输出：

实时状态：State(t, lat, lon, alt_km, vel, tle_epoch, orbit_quality)

轨迹：TrackPoint[]（过去/未来，步长可配）

可选：CZML 用于 Cesium 动画

3) flux_service

抽象接口 FluxModel：

MockFluxModel（可选，用于测试与无 IRENE 环境）

AE9AP9Model（对接 IRENE CLI，需提供可执行程序与命令模板）

能段（至少支持，且可配置扩展）：

电子：Je>100keV、Je>1MeV

质子：Jp>10MeV、Jp>50MeV（可选项）

支持分位数/情景：Mean / P95（Mock 也要模拟该切换）

4) decision_service（核心）

风险分数（必须实现且可配置）：

Risk = Σ wi * (Ji^0.1)

wi、Ji 列表可配置（用户可在网页选择哪些通量参与 Risk）

两级阈值 + 滞回 + 防抖（全部可配置）：

R_off、R_on

hold_time

min_off_time、min_on_time

安全裕度（全部可配置）：

lead_time（提前关机）

lag_time（延后开机）

orbit_quality 差或触发器开 → 裕度放大 factor（可配置）

Fail-safe：

轨道不确定时：缩短预测窗口（如 24h→12h），切保守分位数，输出 reason 标记

输出：

DecisionWindow[]（OBS_ON/OFF、原因、裕度）

command_timeline（JSON 即可，后续可接你们指令系统）

1.3 配置系统（强制）

配置文件：config.yaml（或 json），并提供 schema 校验（Pydantic）

API：

GET /config

PUT /config：更新后立即生效，返回新版本号；支持回滚（保存历史版本）

后端所有阈值/权重/时间参数严禁硬编码

1.4 对外 API（至少）

GET /sat/state（默认返回“当前时刻”实时坐标；也支持传 time）

GET /sat/track?start&end&step

GET /env/flux/track?start&end&step
GET /env/flux/grid?time&channel&percentile&alt_km

GET /decision/windows?start&end

可选实时推送：WebSocket /ws/live（每 1–5 秒推送 state/flux/decision 概要）

2) 前端网页（必须真实可用）

技术栈：Vite + React + TS，3D 地球：CesiumJS。

2.1 页面布局（强制）

左侧侧栏（固定宽度）：参数配置 + 实时坐标信息

右侧主视图（占满剩余空间）：Cesium 3D 地球视图

2.2 左侧侧栏必须包含
A) 实时坐标面板（置顶）

实时显示（每 1–5 秒更新）：

当前时间（UTC 和本地可选）

纬度/经度/高度（km）

速度（可选）

当前使用的 TLE epoch、TLE score、warnings 数量

当前模式：OBS_ON / OBS_OFF

若 OFF：显示 reason + 预计下次恢复时间（来自 windows）

B) 参数配置区（可滚动）

必须实现可编辑并应用的参数（全部来自 /config）：

Risk 权重 wi（对每个通量项提供滑杆 + 数值输入）

选择参与 Risk 的通量项（勾选 Je100/Je1/Jp10/Jp50）

R_off / R_on

hold_time、min_off_time、min_on_time

lead_time、lag_time、放大因子 factor

分位数：Mean/P95

“轨道不确定/触发器时自动更保守”的开关（允许用户覆盖）
按钮（强制）：

“保存并应用”（PUT /config）

“撤销未保存修改”

“恢复默认值”（恢复到 config 初始默认）
并显示：

当前配置版本号

最近一次应用时间
每个参数旁边必须有 tooltip/帮助文本说明其含义。

C) 简要窗口条带（可选但推荐）

显示未来 24h 的 OBS_OFF 窗口条带（小型甘特条/时间条）

2.3 右侧 Cesium 3D 地球必须包含

卫星当前位置 marker

过去/未来轨迹（默认 ±90 分钟，可配置）

轨迹按 Risk（或用户选择的通量项）着色，并显示图例（color legend）

可切换：显示/隐藏轨迹、显示/隐藏风险着色

点击轨迹点：弹出该时刻的 lat/lon/alt + Risk + 主要通量值

2.4 数据刷新与交互

首次加载：拉取 /config、/sat/track、/env/flux/track、/decision/windows

实时模式：轮询或 WebSocket 更新 state，并适当刷新 track/flux（例如每 30–60 秒刷新一次预测段）

调参后：PUT /config 成功 → 立即重新拉取 track/flux/windows 并更新 Cesium 着色

3) 默认配置（必须给出，可直接跑）

预测窗口：24h（轨道不确定时降到 12h）

轨迹显示：过去 90 分钟 + 未来 90 分钟

lead_time：3 分钟；lag_time：6 分钟

hold_time：120 秒

min_off_time：10 分钟；min_on_time：10 分钟

裕度放大 factor：2.0

flux 模型：默认 AE9/AP9（IRENE CLI）

Risk 权重与阈值：给出保守初值并注释

4) 运行与质量要求

必须提供 docker-compose 一键启动（或清晰的本地启动方式）

必须可在本地打开网页看到 3D 地球与卫星轨迹

必须能在网页改参数并立即影响 OBS 窗口与轨迹着色

后端要有最小单元测试：TLE 评分、TLE 选择、决策滞回/防抖、裕度放大

5) 输出要求

输出完整代码与文件树（多文件）

不要只给伪代码

不要把参数写死

在 README 中写明：如何配置 IRENE CLI（exe 与命令模板）、如何切换 Mock/IRENE、如何解释每个参数对观测策略的影响（当前不使用 Space-Track 账号）
