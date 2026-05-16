# 论文笔记：Learning-Based Traffic Classification for Mixed-Critical Flows in Time-Sensitive Networking

## Metadata

- **作者**：Rubi Debnath, Luxi Zhao, Sebastian Steinhorst
- **机构**：TUM (Chair of Embedded Systems and Internet of Things, ESI), Beihang University
- **会议**：IEEE ICC 2025（International Conference on Communications）
- **DOI**：10.1109/ICC52391.2025.11161468
- **在 Debnath 体系中的位置**：这篇是她 LLM preprint（arXiv 2509.26368）里 reference **[2]** —— 也就是说，LLM orchestrator 在 "Traffic Type Assignment" 这一步要调用的"external solver"之一，正是这篇论文里训练出来的 PPO agent

---

## 一句话总结

用 PPO 训一个 RL agent 来决定 TSN 网络里每条 flow 应该被分配为 **TT (Time-Triggered)** 还是 **AVB (Audio-Video Bridging)**，让所有 HRT/SRT 流量都满足 deadline，并且比传统启发式（Tabu Search）快几个数量级。

---

## 问题（Problem）

### 背景

TSN 网络里同时承载三种关键性不同的流量：

- **HRT (Hard Real-Time)**：硬实时，必须严格满足 deadline，否则系统失败（例如工业控制回路、汽车制动）
- **SRT (Soft Real-Time)**：软实时，超过 deadline 性能会降级但不会立即崩
- **NTC (Non-Time-Critical)**：尽力而为流量

TSN 的三种传输服务（Section II 的背景知识）：

- **TT**：通过 GCL (Gate Control List) 在精确时间窗口里独占链路，延迟可预测但需要全网时间同步
- **AVB**：通过 CBS（Credit-Based Shaper）做带宽预留，延迟有上界但不像 TT 那么严格
- **BE**：剩下的带宽给 NTC 用

### TTA 问题（Traffic Type Assignment）

给定一组 mixed-critical flows，对每条流决定它走 TT 还是 AVB —— 这就是 **TTA 问题**。

**为什么难**：

- 是 NP-hard 组合优化问题
- 全部分到 TT：GCL 表项爆炸、调度不可行
- 全部分到 AVB：HRT 流量满足不了 deadline
- 必须根据 flow 的 deadline / period / payload / 路径做精细化分配
- 现有启发式（Tabu Search、ILP）在中大型拓扑上要跑几小时甚至几十小时

---

## 主要贡献（Main Contributions）

1. 把 TTA 问题形式化为一个 **MDP**（state / action / reward 都明确定义）
2. 提出 **TTASelector** —— 一个 PPO-based DRL agent，输入网络状态输出每条 flow 的 TT/AVB 标签
3. 把 RL agent 和两个现有工具拼成完整闭环：
   - **ASAP** 算法：根据 TT 决策合成 GCL（Gate Control List）
   - **NetCal**：用 Network Calculus 计算 AVB 流量的 WCD（Worst-Case Delay）
4. 设计了一套带 **utility shaping** 的 reward 函数，把"满足 deadline"这种二值约束变成连续梯度，让 PPO 能学
5. 在 5 个 synthetic ring 拓扑 + 1 个 realistic Orion CEV 拓扑上验证，比 Tabu Search 基线显著更优

---

## 方法（Methodology）

### MDP 形式化

**State**：

- 每条 flow 的元信息（period, deadline, payload, path）
- 网络的 link capacity matrix（每条链路剩余带宽 / TT 占用情况）
- 这部分论文里没用 GNN，是把整个状态拼成一个固定长度的向量喂给 MLP

**Action**：

- 离散二值：`0 = TT`, `1 = AVB`（Eq. 7）
- 注意：动作空间是 **per-flow** 的，agent 一次决定一条 flow 的 **traffic type**（TT 还是 AVB）—— **路径是给定固定的**（Section II.A："The route of the flows are given and fixed in our model"），agent 不决定路径

**Reward**：Table I 列了 8 种情形（这是这篇论文设计上最讲究的地方）

- 简化地说：
  - HRT 流：必须在 deadline D 之前到达，**没满足 → 强负 reward**
  - SRT 流：用一个 utility function
    - delay ≤ D：full utility = 6
    - D < delay ≤ BD（BD = 1.5×D，soft boundary）：utility 从 6 线性衰减到 0
    - delay > BD：utility = 0（甚至负）
  - NTC：基本不参与奖励，只要不挤占关键流量就行
- 这种 shaping 让 reward 信号不是 sparse 的 0/1，PPO 才能学得动

### Pipeline

```
flow set + topology
        ↓
   TTASelector (PPO)
        ↓
   per-flow TT/AVB label
        ↓
    ├─ TT flows → ASAP → GCL synthesis
    └─ AVB flows → NetCal → WCD calculation
        ↓
   reward = f(HRT满足率, SRT utility, ...)
        ↓
   反向梯度 → 更新 PPO 策略
```

### PPO 超参数（Table II，按论文原文）

- Learning rate **η = 0.0003**
- Discount factor **γ = 0.0001**
- Entropy coefficient **α = 0.0003**
- Clip range **ε = 0.2**
- Value loss coefficient **c1 = 0.5**
- Entropy bonus coefficient **c2 = 0.02**
- **Episodes per run = 1000**, **Steps per episode = 900**
- Evaluation ratio = 4
- Baseline 对照：**A2C**（synthetic 收敛曲线）+ **Tabu Search [Gavriluț & Pop]**（Orion CEV 性能对比）

---

## 结果（Results）

### 测试集

- **5 个 synthetic ring 拓扑**（TC1–TC5），每个拓扑 3 种 flow 数配置 → 共 15 条 synthetic 测试结果（Table III）
- **1 个 realistic Orion CEV**：NASA Orion 载人飞行器的真实通信拓扑（Table IV）

### 核心数字（Orion CEV，Table IV）

| 指标 | Tabu Search [Gavriluț & Pop] | TTASelector |
|------|------|------|
| HRT 可调度率 | 94.94% | **100%** ↑ |
| **SRT utility** | **98.68%** ↑ | 95.98% ↓ |
| 运行时间 | 12:30 (hh:mm) | **3.01 秒** ↓ |

### Synthetic 测试集结果（Table III，TTASelector vs "AVB Only" baseline）

- TTASelector 平均能让 HRT 调度率到 95–100%，多数 case 100%
- SRT utility 普遍 90–98%
- 运行时间 2.3–3.13 秒
- 注意：synthetic 测试**没和 Tabu Search 直接比**，只和"全分 AVB" 这种弱 baseline 比

### Ablation / 对照（Fig. 4）

- **vs. A2C**（Fig. 4a，1000 episodes 收敛曲线）：PPO 更稳、收敛更快、最终 reward 更高
- **PPO 超参数扫描**（Fig. 4b）：测了不同 η/α 组合，验证选定的参数最优
- 论文**没有**对 reward shaping 各项做严格 ablation

---

## 我的理解（My Understanding）

### 这篇论文在 Debnath 体系中的位置

这篇是 LLM preprint 里 reference [2] —— 也就是说，她的 LLM agent vision 里"Step C: Traffic Type Assignment"那一步，**真正的求解器就是这个 PPO agent**。LLM 不是去重新做 TTA，而是 LLM 调度这个 RL solver。


### 为什么用 PPO 而不是其他算法

- TTA 动作空间是离散的 → 排除 DDPG/SAC（连续控制）
- 动作维度很高（per-flow），但每一步动作离散
- PPO 在工业界已经是 default：稳、好调、有 clip ratio 防止策略崩
- A2C 作为 baseline 合理，但 PPO 的 trust region 思想对 reward shaping 多峰这种场景更鲁棒
