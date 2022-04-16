# 矩阵操作
```python
from numpy import * # 矩阵操作库
V = mat([xd,yd,zd]) # 创建13矩阵
V = V.T # 转置
V = V.I # 求逆
print(diag([1,2,2,1])) # 生成对角数组
```

# 解方程
```python
from sympy import solve # 解方程
from sympy.core.symbol import symbols # 定义参数

a0,a1,a2,a3,a4... = sympy.symbols('a0 a1 a2 a3 a4...') # 定义参数
# 列写方程（下面的不全
A = a0 - 20
B = a1 - 0
C = a2 - 0
D = a0 + a1 + a2 + a3 + a4 - 60
# ...

result = sympy.solve([A,B,C,D...],[a0,a1,a2,a3,a4...]) # 求解得到一个字典
theta1 = float(result[a0]) # 取值
for value in result.values(): # 作为数组打印
    print(float(value))
```

# 绘图
```python
import matplotlib.pyplot as plt # 导入绘图库
xs = 2
ys = 6
xf = 12
yf = 3

x = np.linspace(xs,xf,11) # 自变量
y = (yf-ys)/(xf-xs)*(x-xf)+yf # 分解路径为10段

J1 = [2.14,-6.79,-14.6,-21.1,-26.1,-29.7,-32.2,-33.6,-34  ,-33.7,-32.6]
J2 = [139,138 ,136 ,133 ,129 ,125 ,120 ,114 ,108 ,101 ,93.2]

plt.plot(x,J1) # 作图
plt.plot(x,J2)
plt.show() # 显示图片窗口
```