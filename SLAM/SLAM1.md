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

## 3.2旋转向量和欧拉角
- 旋转向量: 方向=旋转轴, 长度=旋转角度 一个三维向量即可描述旋转
- 一次变换: 1旋转向量+1平移向量 六维
- 旋转向量->旋转矩阵: $\textbf{R}=\cos\theta\textbf{I}+(1-\cos\theta)\textbf{nn}^T+\sin\theta n\^{}$  单位长度向量n
- 旋转矩阵->旋转向量: $\textbf{Rn}=\textbf{n}$
- 欧拉角(yaw-pitch-roll ZYX转角)
- 万向锁: ZYX转角定义下, pitch=90°时, 第三次旋转与第一次旋转相同, 丢失了一个自由度(逻辑旋转顺序和实际旋转顺序不一样)

## 3.3四元数
- 四元数 $\textbf{q}=q_0+q_q\textbf{i}+q_2\textbf{j}+q_3\textbf{k}$$~~~~~$ $\textbf{q}=[s,v]^T$
  - 虚部=0:实四元数 实部=0:虚四元数
- 运算
  - 加法 $\textbf{q}_a\pm \textbf{q}_b=[s_a\pm s_b,\textbf{v}_a\pm \textbf{v}_b]^T$
  - 乘法 $\begin{aligned}\boldsymbol{q}_{a} \boldsymbol{q}_{b}=& s_{a} s_{b}-x_{a} x_{b}-y_{a} y_{b}-z_{a} z_{b} \\&+\left(s_{a} x_{b}+x_{a} s_{b}+y_{a} z_{b}-z_{a} y_{b}\right) \mathrm{i} \\&+\left(s_{a} y_{b}-x_{a} z_{b}+y_{a} s_{b}+z_{a} x_{b}\right) \mathrm{j} \\&+\left(s_{a} z_{b}+x_{a} y_{b}-y_{a} x_{b}+z_{a} s_{b}\right) \mathrm{k}\end{aligned}$
    - $\boldsymbol{q}_{a} \boldsymbol{q}_{b}=\left[s_{a} s_{b}-\boldsymbol{v}_{a}^{\mathrm{T}} \boldsymbol{v}_{b}, s_{a} \boldsymbol{v}_{b}+s_{b} \boldsymbol{v}_{a}+\boldsymbol{v}_{a} \times \boldsymbol{v}_{b}\right]^{\mathrm{T}}$
  - 模长 $\left\|\boldsymbol{q}_{a}\right\|=\sqrt{s_{a}^{2}+x_{a}^{2}+y_{a}^{2}+z_{a}^{2}}$
    - $\left\|\boldsymbol{q}_{a} \boldsymbol{q}_{b}\right\|=\left\|\boldsymbol{q}_{a}\right\|\left\|\boldsymbol{q}_{b}\right\|$ 四元数乘积模=模乘积
  - 共轭 $\boldsymbol{q}_{a}^{*}=s_{a}-x_{a} \mathrm{i}-\mathrm{y}_{\mathrm{a}} \mathrm{j}-\mathrm{z}_{\mathrm{a}} \mathrm{k}=\left[\mathrm{s}_{\mathrm{a}},-\mathrm{v}_{\mathrm{a}}\right]^{\mathrm{T}}$
    - 共轭x本身=实四元数 $\boldsymbol{q}^{*} \boldsymbol{q}=\boldsymbol{q} \boldsymbol{q}^{*}=\left[s_{a}^{2}+\boldsymbol{v}^{\mathrm{T}} \boldsymbol{v}, \boldsymbol{0}\right]^{\mathrm{T}}$
  - 逆 $\boldsymbol{q}^{-1}=\boldsymbol{q}^{*} /\|\boldsymbol{q}\|^{2}$
    - 四元数x其逆=1 $q q^{-1}=q^{-1} q=1$
    - 单位四元数: 逆=共轭
    - $\left(\boldsymbol{q}_{a} \boldsymbol{q}_{b}\right)^{-1}=\boldsymbol{q}_{b}^{-1} \boldsymbol{q}_{a}^{-1}$
  - 数乘 $k\textbf{q} = [ks, kv]^T$
- 旋转 三维空间点$p=[0,x,y,z]^T=[0,v]^T$
  - $\textbf{p}'=\textbf{qpq}^{-1}$ 
    - p'纯虚四元数,虚部即坐标
- 四元数->旋转矩阵: $R=vv^T+s^2I+2sv\^{}+(v\^{})^2$
  - $\theta = 2\arccos q_0$
  - $[n_x,n_y,x_z]T=[q_1,q_2,q_3]^T/\sin\frac{\theta}{2}$

## 3.4变换
- 相似变换: 比欧式变换多一个自由度, 允许物体均匀缩放
  - $\boldsymbol{T}_{S}=\left[\begin{array}{ll}s \boldsymbol{R} & \boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right]$
  - 相似变换群: 相似变换的集合 Sim(3)
- 仿射变换: 正交投影, A为可逆矩阵, 立方体不再为方, 各面仍为平行四边形
  - $\boldsymbol{T}_{A}=\left[\begin{array}{ll}\boldsymbol{A} & \boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right]$
- 射影变换: 真实世界->相机照片, 2D 8自由度, 3D 15自由度
- ![在这里插入图片描述](https://img-blog.csdnimg.cn/9cd50cbec8544ad58ac149d5fbaa960d.png)

# 4.李群&李代数
## 4.1.基础
- 旋转矩阵++=特殊正交群$SO(3)$ $\operatorname{SO}(3)=\left\{\boldsymbol{R} \in \mathbb{R}^{3 \times 3} \mid \boldsymbol{R} \boldsymbol{R}^{\mathrm{T}}=\boldsymbol{I}, \operatorname{det}(\boldsymbol{R})=1\right\}$
- 变换矩阵++=特殊欧式群$SE(3)$ $\mathrm{SE}(3)=\left\{\boldsymbol{T}=\left[\begin{array}{cc}\boldsymbol{R} & \boldsymbol{t} \\\mathbf{0}^{\mathrm{T}} & 1\end{array}\right] \in \mathbb{R}^{4 \times 4} \mid \boldsymbol{R} \in \mathrm{SO}(3), \boldsymbol{t} \in \mathbb{R}^{3}\right\}$
- 对加法不封闭,对乘法封闭$\boldsymbol{R}_{1}+\boldsymbol{R}_{2} \notin \mathrm{SO}(3), \quad \boldsymbol{T}_{1}+\boldsymbol{T}_{2} \notin \mathrm{SE}(3)$ $~~~~$ $\boldsymbol{R}_{1} \boldsymbol{R}_{2} \in \mathrm{SO}(3), \quad \boldsymbol{T}_{1} \boldsymbol{T}_{2} \in \mathrm{SE}(3)$
- 群: 一种集合+一种运算的代数结构  集合A 运算· 群G=(A,·)
  - 封闭性 $\forall a_{1}, a_{2} \in A, \quad a_{1} \cdot a_{2} \in A$
  - 结合律 $\forall a_{1}, a_{2}, a_{3} \in A, \quad\left(a_{1} \cdot a_{2}\right) \cdot a_{3}=a_{1} \cdot\left(a_{2} \cdot a_{3}\right)$
  - 幺元 $\exists a_{0} \in A, \quad \text { s.t. } \quad \forall a \in A, \quad a_{0} \cdot a=a \cdot a_{0}=a$
  - 逆 $\forall a \in A, \quad \exists a^{-1} \in A, \quad \text { s.t. } \quad a \cdot a^{-1}=a_{0}$
  - 常见
    - 整数加法$(Z,+)$$~~~~$ 不含0有理数乘法$(Q\setminus 0,·)$
    - 一般线性群GL(n) nxn可逆矩阵, 对矩阵乘法成群
    - 特殊正交群SO(n) 旋转矩阵群 SO(2) SO(3)
    - 特殊欧氏群SE(n) 欧氏变换 SE(2) SE(3)
  - 李群: 连续(光滑)性质的群 SO(n) SE(n) 能连续运动
- 李代数