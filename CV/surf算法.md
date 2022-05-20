# surf算法

## Sift算法
- Sift:一种基于尺度空间的，对图像缩放、旋转、甚至仿射变换保持不变性的图像局部特征描述算子
- [Sift算法](https://blog.csdn.net/dcrmg/article/details/52577555)

1. 特征点提取
   - 特征点是在DOG金字塔尺度空间中提取的，尺度空间的构建涉及到高斯卷积、图像下采样和高斯差分操作。
   - [金字塔尺度空间](https://blog.csdn.net/dcrmg/article/details/52561656)
   - 在尺度空间中先初步提取出在尺度空间和二维图像空间上都是局部极值点的兴趣点，再滤除掉能量低的不稳定的和错误的兴趣点，得到最终稳定的特征点。
2. 特征点描述
   - 特征点方向分配:Sift求取特征点周围邻域内所有像素的梯度方向，生成梯度方向直方图，并归一化为0~360°的梯度方向直方图到36个方向内，取梯度直方图的主要分量所代表的方向作为特征点的方向。
   - 128维向量描述:基于梯度方向直方图展开的，去特征点周围邻域4*4个快，每块提取出8个梯度方向，共计128个方向作为特征的描述子。
3. 特征点匹配
   - 特征点的匹配是通过计算两组特征点的128维的关键点的欧式距离实现的。欧式距离越小，则相似度越高，当欧式距离小于设定的阈值时，可以判定为匹配成功。

- 优点:特征稳定，对旋转、尺度变换、亮度保持不变性，对视角变换、噪声也有一定程度的稳定性；
- 缺点:实时性不高，并且对于边缘光滑目标的特征点提取能力较弱

## Surf算法
- SURF:加速稳健特征，是一种稳健的局部特征点检测和描述算法,改进了特征的提取和描述方式，用一种更为高效的方式完成特征的提取和描述
- [Surf算法](https://blog.csdn.net/dcrmg/article/details/52601010)
1. 构建Hessian（黑塞矩阵），生成所有的兴趣点，用于特征的提取；
2. 构建尺度空间
3. 特征点定位
4. 特征点主方向分配
5. 生成特征点描述子
6. 特征点匹配
- `cv2.xfeatures2d.SURF_create(, hessianThreshold, nOctaves, nOctaveLayers, extended, upright)`
- hessianThreshold：默认100，关键点检测的阈值，越高监测的点越少
- nOctaves：默认4，金字塔组数
- nOctaveLayers：默认3，每组金子塔的层数
- extended：默认False，扩展描述符标志，True表示使用扩展的128个元素描述符，False表示使用64个元素描述符。
- upright：默认False，垂直向上或旋转的特征标志，True表示不计算特征的方- 向，False-计算方向。


## 运行
[示例](https://blog.csdn.net/weixin_29041443/article/details/114546648)
```python
import cv2 as cv
img = cv.imread('./img/001.jpg')
gray= cv.cvtColor(img,cv.COLOR_BGR2GRAY)
surf = cv.xfeatures2d.SURF_create()
kp = surf.detect(gray,None)
img = cv.drawKeypoints(gray, kp, img)
cv.imshow('img', img)
k = cv.waitKey(0)
if k & 0xff == 27:
    cv.destroyAllWindows()
```
[opencv4不支持](https://blog.csdn.net/aiden_yan/article/details/108309503)

[Knnmatch匹配](https://juejin.cn/post/6844904071460028424)
寻找八对最佳匹配
```python
from email.policy import default
import cv2 as cv
import matplotlib.pyplot as plt
surf = cv.xfeatures2d.SURF_create(11200,4,4,False,False)
def FindKeypoints(img):
    kp, desc = surf.detectAndCompute(img, None)
    print("points = "+str(len(kp)))
    img = cv.drawKeypoints(img, kp, None, (0, 255, 0), 4)
    return img,kp,desc
img1 = cv.imread('./img/001.jpg',0)
img1O,kp1,desc1=FindKeypoints(img1)
img2 = cv.imread('./img/002.jpg',0)
img2O,kp2,desc2=FindKeypoints(img2)
bf = cv.BFMatcher()
matches = bf.knnMatch(desc1, desc2,k=2)
good = []
for m,n in matches:
    if m.distance < 0.6*n.distance:
        good.append([m])
print(len(good))
good = sorted(good, key=lambda x: x[0].distance)
for m in good: # 排除掉重复的点对
    for n in good:
        if  n!=m and (m[0].trainIdx == n[0].trainIdx):
            good.remove(n)
img_match = cv.drawMatchesKnn(img1, kp1, img2, kp2, good[:8], None, flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
cv.imwrite("./img/match.png", img_match)
```

![八对最佳匹配](https://img-blog.csdnimg.cn/a28297e6960142a594f987dea3942244.png)

```python
Positions = []
for i in good:
    Positions.append([kp1[i[0].queryIdx].pt,kp2[i[0].trainIdx].pt])
for points in Positions: # 随机颜色重新绘图
    r = random.randint(50, 200)
    g = random.randint(50, 200)
    b = random.randint(50, 200)
    cv.circle(img_match,(int((points[0])[0]),int((points[0])[1])), 10, (r,g,b), -1)
    cv.circle(img_match,(int((points[1])[0])+4272,int((points[1])[1])), 10, (r,g,b), -1)
    cv.line(img_match,(int((points[0])[0]),int((points[0])[1])),(int((points[1])[0])+4272,int((points[1])[1])),(r,g,b),5,-1)
cv.imwrite("./img/match.png", img_match)
```

![随机颜色重新绘图](https://img-blog.csdnimg.cn/8a61f88e868c4f42bd9b4dc05b5996d8.png)


## 八点算法
[SVD分解原理](https://zhuanlan.zhihu.com/p/29846048)

## 本质矩阵
[分解](https://zhuanlan.zhihu.com/p/377794028)
[计算RT](https://zhuanlan.zhihu.com/p/500798616)
