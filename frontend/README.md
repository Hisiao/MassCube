# frontend 前端说明

## 1. 项目概述
`frontend` 目录提供 MASS-Cube 的可视化界面。页面展示实时卫星状态、轨迹风险分段、通量热力图以及参数配置面板，并支持导出未来观测计划 CSV。界面通过定时轮询后端 API 获取轨迹、通量与决策窗口数据。

## 2. 技术栈
- 前端
  - React + TypeScript
  - Vite 构建
  - Cesium + Resium 进行 3D 地球与轨迹渲染
- 后端
  - 依赖 `app/` 提供的 FastAPI 接口（通过 `VITE_API_BASE` 或 Nginx `/api` 反向代理）
- 数据与存储
  - 无本地持久化（数据来自后端 API）
- 开发工具
  - Node.js + npm
  - Vite 开发服务器
  - Nginx（生产部署）

## 3. 项目架构
- API 封装层：`src/api.ts` 统一管理后端请求，支持配置、轨迹、通量、决策窗口与计划导出。
- 状态与渲染层：`src/App.tsx` 负责轮询数据、计算风险序列、生成轨迹分段并叠加通量热力图。
- 类型契约：`src/types.ts` 与后端模型对应，保障前后端数据结构一致。
- 入口与样式：`src/main.tsx` 挂载 React 应用，`src/index.css` 负责全局样式与布局。

## 4. 目录结构
```
frontend/
  src/
    App.tsx               # 主界面与数据流
    api.ts                # 后端 API 调用
    types.ts              # 类型定义
    main.tsx              # 前端入口
    index.css             # 样式
  index.html              # HTML 入口
  vite.config.ts          # Vite 配置与 Cesium 资源路径
  nginx.conf              # Nginx 反向代理配置
  Dockerfile              # 前端构建与部署
  .env                    # 本地环境变量
  .env.example            # 环境变量示例
  package.json            # 依赖与脚本
  tsconfig.json           # TypeScript 配置
  tsconfig.node.json      # Vite/Node 配置
```

## 5. 核心文件说明
- 项目入口文件和配置文件
  - `frontend/index.html`：HTML 入口。
  - `frontend/src/main.tsx`：React 挂载入口。
  - `frontend/vite.config.ts`：Vite + Cesium 构建配置。
  - `frontend/nginx.conf`：生产环境 `/api` 反向代理到后端。
- 核心业务逻辑实现
  - `frontend/src/App.tsx`：状态管理、轮询、风险计算、轨迹分段与热力图叠加。
- 数据模型和 API 接口
  - `frontend/src/types.ts`：前端类型与后端数据结构对齐。
  - `frontend/src/api.ts`：后端 API 访问封装与错误处理。
- 关键组件和服务模块
  - `frontend/src/index.css`：界面主题与布局样式。

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