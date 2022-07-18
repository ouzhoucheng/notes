# 初识SLAM

## 2.1视觉SLAM

- 单目相机
  - 通过视差得知物体的相对远近
  - 尺度不确定性(Scale Ambiguity)
- 双目相机
  - 极几何推算距离
  - 计算量大,需要GPU和FPGA等加速
- 深度相机
  - 物理测量距离,节省算力
  - 范围窄,噪声大,视野小,易受日光干扰,无法测量透射材质

## 2.2经典视觉SLAM框架

![视觉SLAM框架](/img/%E8%A7%86%E8%A7%89SLAM%E6%A1%86%E6%9E%B6.png)

1. **传感器**信息读取: 
   1. 相机图像(编码器,IMU)信息读取和预处理
2. **前端**视觉里程计 Visual Odometry: 
   1. 估算相邻图像间相机的运动和局部地图
   2. 计算机视觉, 图像特征提取匹配
3. **后端**(非线性)优化 optimization: 
   1. 接受不同时刻视觉里程计相机位姿 & 回环检测信息, 处理噪声问题
   2. 优化得全局一致的轨迹 & 地图
   3. 滤波与非线性优化
4. **回环检测** Loop closure: 
   1. 判断机器人是否到达过先前位置
   2. 利用图像间的相似性
5. 建图 mapping
   1. 度量地图 Metric Map: 表示地图中物体位置关系
      1. 稀疏 Sparse: 抽象路标组成, 忽略非路标, 用于定位
      2. 稠密 Dense: 重建所有, 用于导航, 小方块/小格子
   2. 拓扑地图 Topological Map: 节点与边组成, 仅考虑连通性

## 2.3数学建模

- $t=1 ... K$ 离散时刻中发生的事件
- $x_1 ... x_K$ 各时刻的位置
- $y_1 ... y_N$ 路标点
- $u_k$ 运动传感器的读数/输入
- $w_k$ 噪声
- $\mathcal{O}$ 记录k时刻观察到j路标的集合

建模->状态估计问题

- 运动方程 $x_k=f(x_{k-1},u_k,w_k), k=1...K$
- 观测方程 $z_{k,j}=h(y_j,x_k,v_{k,j}), (k,j)\in\mathcal{O}$

分类:

- 线性/非线性系统: 运动,观测方程是否为线性
- 高斯/非高斯系统: 噪声是否服从高斯分布
  - 线性高斯LG系统->卡尔曼滤波KF得出无偏最优估计
  - 非线性非高斯NLNG系统->扩展卡尔曼EKF+非线性优化求解

## 

# 3三维刚体运动

## 3.1旋转矩阵

- 任意向量$a$在基下有坐标 $\boldsymbol{a}=\left[\boldsymbol{e}_{1}, \boldsymbol{e}_{2}, \boldsymbol{e}_{3}\right]\left[\begin{array}{l}a_{1} \\a_{2} \\a_{3}\end{array}\right]=a_{1} \boldsymbol{e}_{1}+a_{2} \boldsymbol{e}_{2}+a_{3} \boldsymbol{e}_{3}$
- 内积 $a\cdot b=a^Tb=\sum_{i=1}^3a_ib_i=|a||b|\cos\lang a,b\rang$
- 外积 $\boldsymbol{a} \times \boldsymbol{b}=\left\|\begin{array}{lll}\boldsymbol{e}_{1} & \boldsymbol{e}_{2} & \boldsymbol{e}_{3} \\a_{1} & a_{2} & a_{3} \\b_{1} & b_{2} & b_{3}\end{array}\right\|=\left[\begin{array}{c}a_{2} b_{3}-a_{3} b_{2} \\a_{3} b_{1}-a_{1} b_{3} \\a_{1} b_{2}-a_{2} b_{1}\end{array}\right]=\left[\begin{array}{ccc}0 & -a_{3} & a_{2} \\a_{3} & 0 & -a_{1} \\-a_{2} & a_{1} & 0\end{array}\right] \boldsymbol{b} \stackrel{\text { def }}{=} \boldsymbol{a}^{\wedge} \boldsymbol{b}$
  - 外积大小 $|a||b|\sin\lang a,b\rang$
  - 反对称阵 $\boldsymbol{a}^{\wedge}=\left[\begin{array}{ccc}0 & -a_{3} & a_{2} \\a_{3} & 0 & -a_{1} \\-a_{2} & a_{1} & 0\end{array}\right]$
- 旋转矩阵
  - $\rightleftarrows$ 行列式=1的正交阵 (逆=转置)
  - n维旋转矩阵的集合 $SO(n)=\{R\in \mathbb{R}^{n\times n}|RR^T=I,det(R)=1 \}$
- 平移矩阵 $a_1=R_{12}a_2+t_{12}$
  - $R_{12}$ :2->1
  - $t_{12}$ :从1到2的向量
- 变换矩阵 $T=\left[\begin{array}{ll}\boldsymbol{R} & \boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right]$
  - 齐次坐标的转换 $c=Ta$
  - 特殊欧氏群 $\operatorname{SE}(3)=\left\{\boldsymbol{T}=\left[\begin{array}{ll}\boldsymbol{R} & \boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right] \in \mathbb{R}^{4 \times 4} \mid \boldsymbol{R} \in \mathrm{SO}(3), \boldsymbol{t} \in \mathbb{R}^{3}\right\}$
  - $\boldsymbol{T}^{-1}=\left[\begin{array}{cc}\boldsymbol{R}^{\mathrm{T}} & -\boldsymbol{R}^{\mathrm{T}}\boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right]$

# 旋转向量和欧拉角
- 旋转向量: 方向=旋转轴, 长度=旋转角度 一个三维向量即可描述旋转
- 一次变换: 1旋转向量+1平移向量 六维
- 旋转向量->旋转矩阵: $\textbf{R}=\cos\theta\textbf{I}+(1-\cos\theta)\textbf{nn}^T+\sin\theta n\^{}$  单位长度向量n
- 旋转矩阵->旋转向量: $\textbf{Rn}=\textbf{n}$
- 欧拉角(yaw-pitch-roll ZYX转角)
- 万向锁: ZYX转角定义下, pitch=90°时, 第三次旋转与第一次旋转相同, 丢失了一个自由度(逻辑旋转顺序和实际旋转顺序不一样)