Metadata

Title: Introduction to Time-Sensitive Networking
Author: Norman Finn（IEEE 802.1 TSN Task Group 主席，时任 Huawei 代表）
Venue / Year: IEEE Communications Standards Magazine, June 2018
DOI: 10.1109/MCOMSTD.2018.1700076
Date read: 2026-05-13

一句话总结
TSN 创始人之一 Finn 写的入门级介绍：把网络服务分为 best-effort / CBR / TSN 三类，论证 TSN 是用普通 Ethernet 硬件实现 "有界延迟 + 零拥塞丢包 + 超高可靠性" 的第三种范式，并把 TSN 拆解成"时间同步 / 合同机制 / 共存机制"三块。
问题
工业控制、音视频制作、汽车控制等应用从来用不了 best-effort 网络（Ethernet）。1980 年代它们被迫发明了专用数字总线（fieldbus），因为 Ethernet：

成本高（相对当时的专用方案）
不可预测（collision detection + retransmission 算法不适合物理控制）

→ 但现在 Ethernet 因为需求量大已经比专用总线便宜了，迁移到 Ethernet 有巨大经济动力——但前提是 Ethernet 能提供工业应用需要的实时性保证。
→ TSN 就是为了让普通 Ethernet 硬件能提供这种保证而生。
主要贡献（这篇 paper 提供的核心概念框架）
1. 三种 packet service 的对比
Service延迟Jitter主要丢包原因Best-effort（Ethernet / IP）统计性，长尾高Congestion（输出 buffer 溢出）CBR（SDH / OTN / TDM）固定≈ 0设备故障（用 1:1 / 1+1 保护）TSN有上限（bounded）有上限多重设备故障（用 FRER 保护）
Fig. 1-3 用概率密度图直观呈现：best-effort 长尾、CBR 几乎一根线、TSN 有窄但有限的分布。
核心 take-away：TSN 是 best-effort 和 CBR 之间的第三条路——保留 best-effort 网络的统计共享 + 灵活性，但对特定 flow 提供 CBR-like 保证。
2. TSN 的三大核心特征

Time Synchronization（时间同步）：所有设备时钟对齐到 1 µs ~ 10 ns 精度，用 IEEE 1588 PTP 变种实现
Contracts（合同机制）：每个 TSN flow 跟网络有合同——transmitter 承诺最大带宽，网络承诺有界延迟和零拥塞丢包
Coexistence（与 best-effort 共存）：在 TSN 流量没占满（设计目标 ≤75%）的前提下，剩余带宽给 best-effort 用，best-effort 的 QoS 机制（priority、WFQ、RED 等）仍然正常工作

3. 零拥塞丢包的核心机制：input rate = output rate
这是这篇 paper 讲得最清楚的部分。原理：

"For every flow, at every hop, over some finite time scale, the input rate equals the output rate."

意思是：每个 hop 上每个 flow 的 transmission opportunity 永远不能 miss，除非源主动减速。
→ 想象网络饱和、flow A 突然停了。某个 switch 的输出端口本来该发 flow A 的包，现在它只能选择 "什么都不发"（或发 best-effort 包），不能把这个时隙挪给 flow B——否则会让 flow B 的下一跳 buffer 溢出。
→ "保留发送时隙，宁可空着也不挪用" 是 TSN 零拥塞丢包的核心原则。
4. 五种 queuing 算法的简明介绍

CBS（Credit-Based Shaper, 802.1Qav）—— 按 credit 限速，最早 AVB 时代的方案
TAS（Time-Aware Shaper, 802.1Qbv）—— Gate Control List，1ns 粒度，最灵活也最难配
Frame Preemption（802.1Qbu + 802.3br）—— 高优先包打断低优先包传输
CQF（Cyclic Queuing and Forwarding, 802.1Qch）—— 输入输出都有 rotating schedule，包按 cycle 整体推进
ATS（Asynchronous Traffic Shaping, P802.1Qcr）—— 那时还是 work in progress，目标是 latency 比 CQF 好、实现成本不太高

注意：MCQF（Debnath 主攻方向）这篇 paper 没提——因为是 2018 年的 paper，MCQF 是更晚才出现的。
5. FRER（Frame Replication and Elimination for Reliability, 802.1CB）

给每个包加序列号
复制成 2 条（或多条）流，走不相交路径
接收端按序列号去重
关键差异：跟传统 CBR 的 1+1 保护比，FRER 是逐包决定用哪一条路径（CBR 的 1+1 是选定一条流忽略另一条）

→ Fig. 4-5 直观展示：单点故障（任意一条路径上一个节点挂了）都不丢包，某些双故障组合（如 A+F、B+C、D+F）也能扛住。
6. TSN 6 种替代方案的局限性
Paper 列了在没 TSN 之前，工业界曾用过的 6 种"曲线救国"方案：

Overprovisioning（过度供应带宽）
Isolation（物理隔离专用网络）
Prioritization（最高优先级）
WFQ（加权公平队列）
Congestion detection（让流量减速——但物理世界不能减速！）
Congestion avoidance（绕开拥塞路径）

→ 这些都有 6 个共同问题：statistical 而非 deterministic、不可预测、corner case 多、缺乏 dynamism、不标准化、太贵。
→ TSN 的本质优势是 deterministic + standardized + dynamic 三合一。
我的理解
1. 这是 TSN 入门的 canonical paper——读了它再读 Zhou 综述会顺很多
Zhou 综述假设读者已经懂 TSN 基础，直接进入 RL-TSN 融合的讨论。但很多概念（TAS 为什么 NP-hard、为什么需要 GCL offline 计算、CQF 为什么解决 scaling 问题）只有读了 Finn 这篇才能真正理解为什么。
理想的读论文顺序应该是：Finn 2018 → Zhou 2026 → Debnath 自己 paper。
我顺序读反了（先读 Zhou），现在补 Finn 这一篇相当于补底层概念。
2. "Input rate = Output rate" 是这篇 paper 最值的一句话
我之前看 Zhou 综述时觉得 "bounded latency" 是个抽象的承诺。读完 Finn 才发现：bounded latency 的本质就是"每个时隙不被挪用"——这一句话比 GCL、CQF、TAS 任何技术细节都更基础。
任何 TSN 调度算法的底层原则都是这一句。
3. Paper 提的 "flexibility" 是 TSN 真正激进的一面

"flexibility, is the most radical change to most existing paradigms for supporting real-time applications over best-effort networks."

我之前没意识到这一点。传统工业网络（PROFINET、EtherCAT）做实时只能 static 配置——任何流量变化都要停网络重配置。
→ TSN 允许动态新增/移除 contract 同时不破坏其他 contract——这是它跟传统 fieldbus 的最大区别。
→ 这跟 Zhou 综述里讲的 "agent traffic 是 dynamic 的，但 GCL 是 static 的" gap 直接关联——Zhou 论证的就是"既然 TSN 设计上允许 dynamic，那 RL 应该来填上这个 dynamic 调度的空缺"。
4. 这篇 paper 是 2018，有些信息已过时
需要心里有数：

ATS 当时是 P802.1Qcr（work in progress），现在是 IEEE 802.1Qcr-2020 已发布
MCQF 当时不存在——是更晚（2020s）才出现的概念
5G-TSN 集成当时刚提（3GPP Release 16 是 2020 年发布的）
AI agent / LLM 完全无关——2018 年没人想这事
IETF DetNet 当时还在 draft 阶段，现在已有 RFC

→ 读这篇就是学经典基础，不指望它讲最新东西。
5. 标准列表（Section "Standards Summary"）是个 reference 速查表
我不应该把 802.1AS / 802.1Q / 802.1Qbv 等等全部记住——但应该知道这张表存在，需要查具体标准时知道这一节有索引。
疑问

75% 设计目标的依据是什么？ Paper 脚注 3 说 "IEEE 802.1 has used 75% as a design goal for the upper limit to the proportion of traffic that is Deterministic"——但没解释为什么是 75% 不是 50% 或 90%。
"FRER 配置成可以应付多重故障" 的具体策略 paper 提了但没展开（Fig. 5 只画了一个例子）。怎么 systematic 设计多路径配置？
CBR 和 TSN 的关键差异是 "TSN packet loss curve has a much lower tail than CBR" —— 这反直觉。我以为 CBR（专用电路）应该可靠性最高？Paper 解释是 CBR 用 1:1 保护，FRER 用 packet-level redundancy，所以 FRER 更细——但为什么 FRER 更细就意味着 lower tail？我没完全理解。
AVB → TSN 的 rebranding（2012） paper 只说 "industrial 和 automotive 社区的需求"驱动了改名，但具体是哪些需求让 AVB 不够用了？

与我论文方向的关联

Finn 这篇是 Debnath 研究的"史前史" ——她做的 CQF、MCQF、Frame Preemption、ATS 这些都是 Finn 在这篇 paper 里列出的基础机制。Finn 给了机制清单，Debnath 在做某个机制的优化。
"Input rate = output rate" 是任何 TSN scheduling RL paper 的 invariant 约束——RL 学的 policy 必须满足这个约束，否则会破坏 deterministic 保证。这点跟我之前 paper 笔记里的 "Section IV 关于 RL 和 TSN 在不同时间尺度分离"是同一回事。
AVB → TSN 的历史 解释了为什么我会在文献里看到很多 "AVB" 的术语——AVB 是 TSN 的前身，有些机制（CBS、802.1AS 时间同步）在 AVB 时代就有了，TSN 是在 AVB 基础上扩展。
Paper 列的 6 种 TSN 替代方案 给我一个 baseline——如果我 thesis 需要解释"为什么不用更简单的方案"，这 6 个就是经典反例。