TSN 核心机制
TSN (Time-Sensitive Networking) —— IEEE 802.1 工作组的一系列标准，给 Ethernet 加确定性通信能力（bounded latency + minimal jitter + high reliability）。是 layer 2 技术。
TAS (Time-Aware Shaper, IEEE 802.1Qbv) —— 用 Gate Control List 控制不同优先级队列的发送时间窗口，提供 microsecond 级硬实时延迟保证。但 GCL 通常 offline 计算、static——这就是 RL 优化的核心动机。
CBS (Credit-Based Shaper, IEEE 802.1Qav) —— 给每个优先级队列发 "credit"，只有 credit 为正时才能传输。比 TAS 简单，保证 bandwidth 但不保证 strict timing。
CQF (Cyclic Queuing and Forwarding, IEEE 802.1Qch) —— 把时间分成周期 cycle，第 N 周期进的包在第 N+1 周期发出。提供 bounded latency 同时实现简单。
MCQF —— CQF 的 multi-cycle 扩展，多个 cycle 长度组合在一起。
Frame Preemption (IEEE 802.1Qbu / 802.3br) —— 高优先级帧可以中断正在传输的低优先级帧，传完高优先级再恢复低优先级。减少 head-of-line blocking。
ATS (Asynchronous Traffic Shaping, IEEE 802.1Qcr) —— 用 token bucket 做 per-flow 整形，不需要全网时间同步。
FRER (Frame Replication and Elimination for Reliability, IEEE 802.1CB) —— 关键帧在多条不相交路径上同时发，接收端去重。零丢包失效切换。
PSFP (Per-Stream Filtering and Policing, IEEE 802.1Qci) —— 进入侧的流量整形和过滤，隔离故障节点防止 QoS 退化。
GCL (Gate Control List) —— TAS 用的"时刻表"，规定每个时间窗哪个优先级队列的 gate 开/关。
gPTP (generalized Precision Time Protocol, IEEE 802.1AS) —— TSN 用的网络时钟同步协议，sub-microsecond 精度。所有 TSN bridge 时钟对齐就靠它。
CNC (Centralized Network Controller) —— TSN 网络的"大脑"，知道全网拓扑、算 GCL、推配置到所有 switch。
CUC (Centralized User Configuration) —— 收集 end station（终端设备）的流量需求，传给 CNC。
TSN Bridge —— 支持 TSN 的交换机。
Stream —— TSN 里的 "flow"，从 source 到 destination 的一连串相关 packet。

RL 核心算法
RL (Reinforcement Learning) —— Agent 跟环境交互，从 reward 中学最优策略的机器学习范式。
DRL (Deep RL) —— RL + 深度神经网络，处理高维 state/action space。
MARL (Multi-Agent RL) —— 多个 RL agent 在共享环境中协作或竞争。
MDP (Markov Decision Process) —— RL 的数学框架，由 (S, A, P, r, γ) 五元组组成：状态、动作、转移概率、奖励、折扣因子。
DQN (Deep Q-Network) —— 用神经网络近似 Q 值函数的 value-based RL。适合 discrete action space。
PPO (Proximal Policy Optimization) —— On-policy policy gradient 方法，训练稳定。适合连续动作。
SAC (Soft Actor-Critic) —— Off-policy 最大熵 RL，sample 效率高。
DDPG / TD3 —— Deterministic policy gradient 方法，连续控制。
MADDPG —— DDPG 的多 agent 版本，centralized critic + decentralized actor。
QMIX —— MARL 算法，把每个 agent 的 Q 值通过 monotonic mixing network 组合成全局 Q。
CTDE (Centralized Training, Decentralized Execution) —— MARL 主流范式：训练时所有 agent 共享信息，执行时各自独立。
Bellman Optimality Equation —— Q-learning 的核心方程，描述最优 Q 值的递归关系。
GNN (Graph Neural Network) —— 在图结构数据上做学习的神经网络。TSN 拓扑天然是图，GNN 很适合用来表示。

Industrial AI Agent
Industrial AI Agent —— 工业场景的自主智能体，有感知/记忆/规划/执行四个认知模块，跟传统工业 agent 比是用 LLM/DL 驱动的。
MAS (Multi-Agent Systems) —— 多 agent 协作的系统范式。工业里从 1990 年代就有，是 industrial agent 的前身。
MCP (Model Context Protocol) —— Anthropic 提的 agent 协议，让 LLM 调用工具。基于 JSON-RPC，best-effort 延迟。
A2A (Agent-to-Agent) —— Google 提的 agent 间通信协议。HTTP/SSE 传输。
LLM (Large Language Model) —— 大语言模型。这篇 paper 强调 LLM 不是 thesis 主题，而是工具——比如用 LLM 翻译意图、生成 reward function 等。
RAG (Retrieval-Augmented Generation) —— 给 LLM 配检索系统，让它能查外部知识库。
Cognitive Architecture —— 这篇 paper 强调的核心概念：agent 有 perception / memory / planning / execution 四模块，每个模块产生的网络流量特征完全不同。
Perception Module —— Field-level 网络，高带宽 streaming（视觉、激光雷达、触觉）。
Memory Module —— MES-level 网络，bursty 高带宽 spike（数据库查询）。
Planning Module —— ERP-level 网络，bursty 推理 traffic。
Execution Module —— Controller-level 网络，periodic motion + event-trigger，低 jitter + 高同步。
工业系统术语
ISA-95 / IEC 62264 —— 工业自动化的层级架构标准：L0-L2 是物理层 / 控制层（fieldbus + TSN），L3 是 MES，L4 是 ERP。
OPC UA —— 工业通信标准，跟 TSN 结合做 PubSub。
PROFINET / EtherCAT / CC-Link IE —— 传统工业 fieldbus 协议，可以跑在 TSN 之上。
MES (Manufacturing Execution System) —— 制造执行系统。
ERP (Enterprise Resource Planning) —— 企业资源规划。
PLC (Programmable Logic Controller) —— 可编程逻辑控制器。
DCS (Distributed Control System) —— 分布式控制系统，过程工业用。
AGV (Automated Guided Vehicle) —— 自动导引车，仓储物流标配。
SIL (Safety Integrity Level) —— 安全完整性等级，2-4 是工业安全要求。

网络/系统术语
QoS (Quality of Service) —— 服务质量，延迟、jitter、可靠性等。
KPI (Key Performance Indicator) —— 关键性能指标。
URLLC (Ultra-Reliable Low-Latency Communication) —— 5G 的高可靠低延迟服务。
AoI / AoS (Age of Information / Age of Synchronization) —— 信息新鲜度指标，描述数据已经"多老"。AoS 是这篇 paper 提到的，用于 5G-TSN uplink。
5G-TSN integration —— 5G 系统作为 TSN bridge，在 wireless 域提供 TSN 保证。3GPP Release 16+ 标准化。
SD-TSN (Software-Defined TSN) —— 用 SDN 范式做 TSN，控制面集中、可重配置。
DetNet —— IETF 的 Deterministic Networking 工作组，layer 3 的确定性网络（TSN 是 layer 2）。
FRER (Frame Replication and Elimination for Reliability) —— 见上面 TSN 部分。