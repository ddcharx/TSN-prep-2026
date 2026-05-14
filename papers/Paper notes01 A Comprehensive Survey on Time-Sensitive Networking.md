## Metadata
- **Title**: A Comprehensive Survey on Time-Sensitive Networking
- **Authors**: Nan Zhou et al. (Zhou, Yao, Liang, Yao, Wan, Yang, Gui, Gao)
- **Venue / Year**: IEEE Communications Surveys & Tutorials, 2026
- **DOI**: [10.1109/COMST.2026.3690893](https://doi.org/10.1109/COMST.2026.3690893)
- **Date read**: 2026-05-12

## 一句话总结
这篇综述系统性论证：未来工业 AI agent 通信需要 RL 的"自适应优化"和 TSN 的"确定性保证"融合，并提出一个四维度（智能调度 / 动态资源管理 / 网络融合与移动性 / 应用感知网络）的 agent-centric 框架来分析现有研究的 gap、挑战和未来方向。

## 问题
工业 AI agents（智能制造场景下的具身智能体，如视觉引导机器人、AGV、过程控制 agent）的通信需求同时要求两件相互矛盾的事：

"strict 确定性：100µs - 10ms 延迟（场景依赖，最严苛场景 sub-millisecond）、microsecond 级 jitter、99.999% - 99.9999% 可靠性"
adaptive 智能：能根据 agent 的感知-记忆-规划-执行认知循环动态调整

现有方案都解决不了：

传统工业网络（PROFINET、EtherCAT）只能 static 配置，无法适应动态流量
现代 AI agent 协议（MCP、A2A）基于 HTTP/JSON-RPC，是 best-effort，延迟 100ms-1s 量级
现有 RL+TSN 研究 network-centric，把 agent 当一般 flow，不理解 agent 认知架构

为什么重要：Industry 4.0 → 5.0 的转型下，工业 AI agent 数量爆炸，但没有合适的通信基础设施支持它们。

## 主要贡献

第一次从 agent-centric 视角分析 RL-TSN 融合——而不是把 agent 当成一般 network user

RL agent（算法）一直在优化网络
但它不知道 industrial AI agent（机器人）现在在干嘛
比如机器人正在做 "感知 → 规划 → 执行"循环，每个阶段流量需求完全不同
现有 RL 都把这种流量看成 "随机突发流"，不知道这种突发是有规律的

paper 提的"agent-centric perspective"就是：让 RL agent 知道 industrial AI agent 的认知状态。
四维度框架：

D1 Intelligent Scheduling（智能调度——"什么时候发"）
D2 Dynamic Resource Management（动态资源管理——"分多少"）
D3 Network Convergence & Mobility（网络融合与移动性——"在哪里跨域"）
D4 Application-Aware Networking（应用感知网络——"为什么这条优先"）

Gap analysis：现有所有 RL-TSN 工作 agent awareness 都是 None 或 Limited——这是个系统性盲区
整合 4 个不同社区（RL for networking / TSN / Edge AI / Agent protocols）的文献并指出每个社区的 limitation
研究路线图：1-2 年短期 / 3-5 年中期 / 5-10 年长期方向

## 整理文献的方式：

"建立 agent 认知架构（感知-记忆-规划-执行 4 个模块）作为分析基础——根据 ISA-95 工业层级的约束（Levels 0-2 控制网络 vs Levels 3-4 管理网络），把 4 个模块映射到 Field / MES / ERP / Controller 4 种网络层级"
MDP/RL 公式化对比：把 ~35 篇 RL+TSN 论文的 state / action / reward 列成表格（Table IX、X）这是这篇 survey 最有价值的部分之一
按 4 维度分类所有现有工作（Fig. 6 的 taxonomy）
三步分析每个维度：现有工作 → 挑战 → 需求

核心论点：RL 和 TSN 看似矛盾（一个 stochastic、一个 deterministic），但在不同时间尺度上协同工作：

TSN 在数据面（微秒级）保证每个 packet 的确定性
RL 在控制面（毫秒-秒级）学最优调度策略
这种架构上和时间上的分离让两者不冲突


## 结果
(综述无实验结果，但他们整理出几个量化对照)
工业 vs IT agent 通信需求差距（Table I、VII）：

Latency：工业 100µs - 10ms，IT 100ms-1s → 100-1000× gap
Jitter：工业 <10μs，IT unbounded → critical
Reliability：工业 99.999% - 99.9999%（场景依赖），IT 99-99.9% → significant gap

代表性 RL+TSN 工作的性能（来自他们整理的表 XI-XIV）：

DeepScheduler [31]：36% schedulability + 150× 速度（vs heuristic）
GTSNet [123]：cross-topology 泛化，减少 50%+ failed flows
DecAge [92]：5G-TSN 分布式 age-aware 调度
Yao et al. [142]：8.89-39.15% 延迟降低 + 39.73% jitter 降低

"但所有这些工作的 'Agent Awareness' 都是 None 或 Limited—，这是 survey 的核心 finding。D4 维度（应用感知）相对其他维度有更多 'Limited' 标记，但没有任何一个工作达到 Full。"

## 我的理解

为这一篇同时覆盖了 TSN 基础 + RL/ML 在 TSN 的应用 + AI agent 通信。三个领域一次性给我。
推测：她研究方向（ML/LLM in TSN）可能踩在这篇 paper 的 D1/D2/D4 维度的交叉：

她做的 TSN scheduling = D1
她做的 ML for TSN = D1 和 D2 都涉及
她可能在做的 LLM for TSN = D4 应用感知网络的延伸

这篇 paper 提出的"agent-centric gap"可能是 BSc 题目方向，做某种"考虑 agent 认知模块的调度算法"，而不是又一个 generic ML for TSN。
paper 提到"Protocol Language Models (PLMs)"作为未来方向，这跟她说的 ML/LLM in TSN 相似。LLM 不是用来 generate 文本，而是用来理解异构工业协议。
paper 的四维度框架给了我一个非常清晰的领域地图——我以后选题、读 reference paper都能用这个 framework 定位。


## 疑问

RL 和 TSN 真的能完美分离吗？ paper 说"control plane / data plane separation" 让 RL 不影响 TSN 的实时性，但实际部署时 RL 更新 GCL 时如何避免破坏 in-flight traffic？Paper 提到 shadow mode 和 formal verification 等机制，但没有给具体的 hitless GCL update 实现细节
"agent-aware"具体怎么 encode 到 RL state space？ paper 一直在说要把 agent 的认知模块/任务 phase 当 state，但没有给具体的 encoding 方式。
MARL 把 industrial agents 而不是 switches 当 RL agents 这个想法 paper 反复提，但没有一个现有工作真的这么做了
Section V.B.4 GenAI-Enhanced RL 提到用 LLM 做 RL 的 reward designer、feature extractor、policy interpreter 具体怎么操作？
不过这全都是具体实现方法上的问题，不是这个综述该干的

## 附
TSN 的核心价值就是消除 jitter
普通以太网：jitter 几十到几百 μs（高优先包卡在低优先包后面等）
TSN 用 TAS、CQF、Frame Preemption 等机制：jitter 压到 sub-μs 级别
具体怎么做的：

TAS：用 Gate Control List 给关键流量预留固定时刻——packet 永远在同一时刻发，自然无 jitter
Frame Preemption：高优先包可以打断低优先包传输——不用等
gPTP：全网时钟同步到 sub-μs——大家步调一致

TSN 之所以重要，本质就是因为它第一次在 Ethernet 上做到了 deterministic jitter——以前只有 EtherCAT/PROFINET 这种专用 fieldbus 能做到。