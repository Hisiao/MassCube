你是一名资深航天任务软件工程师 + 后端架构师 + 科学计算工程师。请为一个卫星任务（NORAD ID = 65490，COSPAR = 2025-197C）生成可运行的工程代码（以生产可用为目标），实现从 Space-Track 获取 TLE、用 SGP4 生成实时轨道、再计算电子/质子通量风险并输出观测规避/关机窗口，同时提供可视化所需的数据接口。

0. 目标与约束

目标：保障 0.1–10 MeV 伽马谱线观测不受高通量背景影响；当进入高通量区（范艾伦辐射带等）时可自动规避并控制载荷关机。

轨道来源：TLE/SGP4（高精度上限受 TLE 限制，因此必须输出“轨道质量/不确定性提示”，并把不确定性转化为安全裕度）。

通量模型：以 AE9/AP9/SPM（IRENE）为主，但考虑到环境依赖复杂，代码结构必须允许：

真正对接 IRENE（外部可执行文件或库调用）

先用 Mock/占位实现（返回可控的通量值，用于联调 UI/决策）

关键要求：所有阈值、权重、滞回、提前/延后时间、模式切换条件，都必须放在配置文件中，并提供运行时热更新接口（例如 REST API 修改后即时生效，且可回滚）。

1. 系统架构（必须按模块实现）

实现一个最小可用但可扩展的服务端系统（建议 Python）：

1.1 tle_service

主源：Space-Track（需要用户名/密码/令牌等，从环境变量读取）。

备源：CelesTrak（当主源失败/超时/限流时自动降级）。

功能：

按 NORAD=65490 拉取最新 TLE，并保存历史（建议最近 14 天）。

TLE 质量门控（必须实现评分/告警）：

行校验和与解析完整性

新鲜度：now-epoch 超阈值则降分/告警

与上一条对比：关键要素变化异常告警（平均运动、倾角、RAAN、偏心率等）

传播偏差检查：旧 TLE 外推到新 epoch 与新 TLE 在同一时刻状态差超阈值告警

输出：标准化对象 TLESet（包含 source、fetched_at、epoch、score、警告列表等）

1.2 orbit_service

使用 Vallado SGP4 实现（python-sgp4）。

关键点：

“按时间选最优 TLE”：对任意时刻 t，选择 epoch 最接近且评分最高的 TLE 来传播，而不是永远用最新。

坐标转换：必须把 SGP4 输出的 TEME 正确转换到 ECEF/经纬高（允许使用成熟库/实现，但要写清楚依赖与验证方法）。

输出：

实时状态：State(t, lat, lon, alt, v, tle_epoch, orbit_quality)

轨迹点串：用于前端（JSON 或 CZML）

1.3 flux_service

输入：轨道状态串（时间、位置、高度）以及用户选择的能段/通量定义。

输出：电子/质子通量 + 风险值 Risk（可解释）

实现方式：

抽象接口 FluxModel：

AE9AP9Model（真实对接 IRENE 的适配器，允许先留 TODO）

MockFluxModel（可配置的假数据/区域模型，用于先跑通全链路）

能段：至少支持以下积分通量（可配置添加更多）：

电子：Je>100keV、Je>1MeV

质子：Jp>10MeV、Jp>50MeV（可选）

支持分位数/情景：默认 Mean，可切 P95；当空间天气触发器生效时自动切更保守分位数（仍允许用户手动覆盖）。

1.4 decision_service（核心）

输入：flux_timeseries + orbit_quality + 外部空间天气触发器状态（例如 Kp）

输出：

观测窗口/关机窗口：[t_start, t_end, mode=OBS_ON/OFF, reason, margins]

可直接喂给“指令生成器”的 command_timeline（先用 JSON 表达即可）

决策规则（必须实现且全部可配置）：

风险分数：

Risk = Σ wi * log10(Ji)（wi 可配置，Ji 选择哪些通量也可配置）

两级阈值 + 滞回：

Risk > R_off 进入保护（关机/停止观测）

Risk < R_on 且持续 hold_time 才允许恢复

min_off_time、min_on_time 防抖

安全裕度：

lead_time 提前关机

lag_time 延后开机

当 orbit_quality 变差或空间天气触发器为真时：自动放大裕度（例如 *2）

Fail-safe：

当 TLE 质量告警/疑似机动：缩短预测窗口、强制更保守分位数、并在输出中标记“轨道不确定”。

2. 可配置与热更新（强制）

必须实现统一配置系统（推荐 YAML/JSON + 环境变量覆盖）：

orbits: 刷新率、未来预测时长、历史窗口时长、TEME->ECEF 方法开关等

tle_quality: 新鲜度阈值、要素变化阈值、传播偏差阈值、评分权重

flux: 能段列表、分位数默认值、模型选择（AE9/AP9 或 Mock）

decision: Risk 权重 wi、R_off/R_on、hold_time、min_on/off、lead/lag、放大因子、触发器阈值（Kp 等）

ui: 轨迹长度、采样间隔、色条范围（可选）

并提供 API：

GET /config 查看当前配置

PUT /config 更新配置（带 schema 校验 + 版本号 + 回滚能力）

所有服务读取同一份配置（或通过配置服务下发），更新后立即生效。

3. 数据接口（给前端动态显示用）

至少提供：

GET /sat/state?time=... 返回某时刻卫星位置与质量指标

GET /sat/track?start=...&end=...&step=... 返回轨迹点串

GET /env/flux/track?... 返回沿轨通量与 Risk 时间序列

GET /decision/windows?... 返回观测 ON/OFF 窗口

可选：GET /env/tiles?... 返回全球热力图瓦片索引（先可 stub）

4. 默认参数（必须写入配置，且易于用户修改）

给出一套“保守但可用”的默认值（用户可调整）：

预测窗口：24h（轨道不确定时降到 12h）

lead_time：3 分钟；lag_time：6 分钟

hold_time：120 秒

min_off_time：10 分钟；min_on_time：10 分钟

分位数：默认 Mean；触发器开启时切 P95

Risk 权重：默认给一组合理初值（明确写在配置里，并解释意义）

放大因子：orbit_quality 差或触发器开 → lead/lag *2

5. 工程交付与质量要求

输出一个完整项目结构（例如 FastAPI + Uvicorn），含：

依赖清单、README、配置示例、启动方式

数据模型（Pydantic/dataclass）

日志（结构化日志，记录 TLE source、epoch、score、决策原因）

健康检查：/healthz

单元测试：至少覆盖

TLE 校验与评分

“按时间选最优 TLE”的选择逻辑

决策滞回、防抖、lead/lag 行为

必须清晰标注哪些部分是 Mock，如何切换到真实 AE9/AP9。

6. 不能做的事

不要把任何阈值/权重写死在代码里（必须配置化）。

不要只输出伪代码；要输出可运行的工程代码与文件结构。

不要省略坐标转换验证说明与最小测试。

如果你对输出语言/框架需要我指定：请默认 Python + FastAPI；后台任务用 APScheduler 或 Celery（二选一即可）；配置用 YAML + schema 校验；存储可以先用 SQLite/PostgreSQL 任选其一（给出可替换接口）；缓存可选 Redis（没有也要能跑）。