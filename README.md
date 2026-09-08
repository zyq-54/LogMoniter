```markdown
Permalink: ```markdown
# 基于 C++ 和 AI 的实时日志监控系统
Permalink: # 基于 C++ 和 AI 的实时日志监控系统
￼
Permalink: 
## 📌 项目简介
Permalink: ## 📌 项目简介
￼
Permalink: 
这是一个从零构建的轻量级日志采集、存储、监控与智能分析系统。
Permalink: 这是一个从零构建的轻量级日志采集、存储、监控与智能分析系统。
它实现了多线程日志接收、按级别过滤、文件轮转、实时错误告警，并集成了大模型 API，在错误触发时自动进行根因分析。
Permalink: 它实现了多线程日志接收、按级别过滤、文件轮转、实时错误告警，并集成了大模型 API，在错误触发时自动进行根因分析。
￼
Permalink: 
**核心目标**：模拟真实生产环境下的可观测性数据管道，并引入 AI 辅助排障能力。
Permalink: **核心目标**：模拟真实生产环境下的可观测性数据管道，并引入 AI 辅助排障能力。
￼
Permalink: 
---
Permalink: ---
￼
Permalink: 
## 🏗️ 系统架构
Permalink: ## 🏗️ 系统架构
￼
Permalink: 
```text
Permalink: ```text
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
Permalink: ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ C++ 客户端 │ ──▶ │ C++ 服务端 │ ──▶ │ output.txt │
Permalink: │ C++ 客户端 │ ──▶ │ C++ 服务端 │ ──▶ │ output.txt │
│ (模拟数据) │ │ (多线程TCP) │ │ (日志文件) │
Permalink: │ (模拟数据) │ │ (多线程TCP) │ │ (日志文件) │
└─────────────┘ └─────────────┘ └─────────────┘
Permalink: └─────────────┘ └─────────────┘ └─────────────┘
│
Permalink: │
▼
Permalink: ▼
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
Permalink: ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ AI 分析模块 │ ◀── │ Python 监控 │ ◀── │ 定时读取 │
Permalink: │ AI 分析模块 │ ◀── │ Python 监控 │ ◀── │ 定时读取 │
│ (DeepSeek) │ │ (错误统计) │ │ 最近日志 │
Permalink: │ (DeepSeek) │ │ (错误统计) │ │ 最近日志 │
└─────────────┘ └─────────────┘ └─────────────┘
Permalink: └─────────────┘ └─────────────┘ └─────────────┘
```
Permalink: ```
￼
Permalink: 
---
Permalink: ---
￼
Permalink: 
## 🚀 快速开始
Permalink: ## 🚀 快速开始
￼
### 1. 编译 C++ 服务端
Permalink: ### 1. 编译 C++ 服务端
￼
```bash
Permalink: ```bash
# 使用 Visual Studio 打开 .sln 文件，直接编译
Permalink: # 使用 Visual Studio 打开 .sln 文件，直接编译
# 或使用 g++ 编译（需链接 ws2_32.lib）
Permalink: # 或使用 g++ 编译（需链接 ws2_32.lib）
g++ -o log_server.exe log_server.cpp -lws2_32 -std=c++17
Permalink: g++ -o log_server.exe log_server.cpp -lws2_32 -std=c++17
```
￼
### 2. 启动服务端
Permalink: ### 2. 启动服务端
￼
```bash
Permalink: ```bash
log_server.exe
Permalink: log_server.exe
```
￼
### 3. 运行监控 + AI 分析脚本
Permalink: ### 3. 运行监控 + AI 分析脚本
￼
```bash
Permalink: ```bash
# 安装依赖
Permalink: # 安装依赖
pip install requests pandas scikit-learn
Permalink: pip install requests pandas scikit-learn
￼
# 启动监控
Permalink: # 启动监控
python ai_monitor.py
Permalink: python ai_monitor.py
```
￼
### 4. 测试数据
Permalink: ### 4. 测试数据
￼
```bash
Permalink: ```bash
# 使用测试客户端发送 200 条混合日志
Permalink: # 使用测试客户端发送 200 条混合日志
./test_client.exe
Permalink: ./test_client.exe
```
￼
---
Permalink: ---
￼
## 🧰 技术栈
Permalink: ## 🧰 技术栈
￼
| 模块 | 技术 | 说明 |
Permalink: | 模块 | 技术 | 说明 |
|------|------|------|
Permalink: |------|------|------|
| 服务端 | C++17、Winsock2、STL | 多线程 TCP 服务器，按行过滤 \[ERROR] |
Permalink: | 服务端 | C++17、Winsock2、STL | 多线程 TCP 服务器，按行过滤 \[ERROR] |
| 存储 | 文件系统 | 日志按 output.txt 存储，支持轮转（100MB） |
Permalink: | 存储 | 文件系统 | 日志按 output.txt 存储，支持轮转（100MB） |
| 监控 | Python、Pandas | 每 5 秒读取最近 60 秒日志，统计错误数 |
Permalink: | 监控 | Python、Pandas | 每 5 秒读取最近 60 秒日志，统计错误数 |
| AI 分析 | DeepSeek API、requests | 错误超标时自动调用大模型进行根因分析 |
Permalink: | AI 分析 | DeepSeek API、requests | 错误超标时自动调用大模型进行根因分析 |
| 可视化 | Matplotlib | 绘制错误趋势图（可选） |
Permalink: | 可视化 | Matplotlib | 绘制错误趋势图（可选） |
￼
---
Permalink: ---
￼
## 💡 项目亮点
Permalink: ## 💡 项目亮点
￼
- **完整链路**：采集 → 存储 → 监控 → AI 分析，闭环实现。
Permalink: - **完整链路**：采集 → 存储 → 监控 → AI 分析，闭环实现。
- **工程规范**：多线程同步、文件轮转、异常处理、编码兼容（UTF-8/GBK）。
Permalink: - **工程规范**：多线程同步、文件轮转、异常处理、编码兼容（UTF-8/GBK）。
- **智能决策**：结合 DeepSeek 大模型，实现“报警 + 诊断”一体化。
Permalink: - **智能决策**：结合 DeepSeek 大模型，实现“报警 + 诊断”一体化。
- **轻量部署**：单机运行，不依赖任何外部服务（数据库、消息队列）。
Permalink: - **轻量部署**：单机运行，不依赖任何外部服务（数据库、消息队列）。
￼
---
Permalink: ---
￼
## 📁 目录结构
Permalink: ## 📁 目录结构
￼
```text
Permalink: ```text
.
├── cpp_server/
Permalink: ├── cpp_server/
│ ├── log_server.cpp # 主服务
Permalink: │ ├── log_server.cpp # 主服务
│ └── test_client.cpp # 测试客户端
Permalink: │ └── test_client.cpp # 测试客户端
├── python_monitor/
Permalink: ├── python_monitor/
│ ├── ai_monitor.py # 监控 + AI 分析主程序
Permalink: │ ├── ai_monitor.py # 监控 + AI 分析主程序
│ └── history.csv # 错误计数历史（自动生成）
Permalink: │ └── history.csv # 错误计数历史（自动生成）
├── output.txt # 日志文件
Permalink: ├── output.txt # 日志文件
└── README.md
Permalink: └── README.md
```
￼
---
Permalink: ---
￼
## 📸 运行效果
Permalink: ## 📸 运行效果
￼
> 终端输出示例：
Permalink: > 终端输出示例：
```text
Permalink: ```text
[ALERT] 过去 60 秒内错误超过 5 条！
Permalink: [ALERT] 过去 60 秒内错误超过 5 条！
🤖 AI 分析结果：
Permalink: 🤖 AI 分析结果：
经过分析，错误可能是由于客户端连接频繁超时导致，
Permalink: 经过分析，错误可能是由于客户端连接频繁超时导致，
建议检查网络配置或增加连接池大小。
Permalink: 建议检查网络配置或增加连接池大小。
```
￼
---
Permalink: ---
￼
## 🧠 后续计划
Permalink: ## 🧠 后续计划
￼
- [ ] 支持配置文件（端口、阈值、AI Key）
Permalink: - [ ] 支持配置文件（端口、阈值、AI Key）
- [ ] 增加 Web 仪表盘（Flask + ECharts）
Permalink: - [ ] 增加 Web 仪表盘（Flask + ECharts）
- [ ] 支持多日志源（HTTP、Kafka）
Permalink: - [ ] 支持多日志源（HTTP、Kafka）
￼
---
Permalink: ---
￼
## 📬 联系我
Permalink: ## 📬 联系我
￼
- GitHub：ZYQ-54
Permalink: - GitHub：ZYQ-54
- 邮箱：2889522868@qq.com
Permalink: - 邮箱：2889522868@qq.com
￼
---
Permalink: ---
## ⭐ 如果这个项目对你有帮助，欢迎 Star ✨
Permalink: ## ⭐ 如果这个项目对你有帮助，欢迎 Star ✨
```
