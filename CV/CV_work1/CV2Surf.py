from turtle import position
import cv2 as cv
import matplotlib.pyplot as plt
import random
import numpy as np
def Find8BestPoints(img1,img2):
    # surf算法寻找特征点
    surf = cv.xfeatures2d.SURF_create(11200,4,4,False,False)
    kp1, desc1 = surf.detectAndCompute(img1, None)
    kp2, desc2 = surf.detectAndCompute(img2, None)
    # bf匹配器
    bf = cv.BFMatcher()
    matches = bf.knnMatch(desc1, desc2,k=2)
    # 寻找优质匹配
    good = []
    for m,n in matches:
        if m.distance < 0.6*n.distance:
            good.append([m])
    # print(len(good))
    good = sorted(good, key=lambda x: x[0].distance) # 排序
    # 排除掉重复的点对,取出八个最优的
    for m in good: 
        for n in good:
            if  n!=m and (m[0].trainIdx == n[0].trainIdx):
                good.remove(n)
    good = good[:8] # 取出八个最优的
    # 匹配作图和保存
    img_match = cv.drawMatchesKnn(img1, kp1, img2, kp2, good, None, flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
    # cv.imwrite("./img/match.png", img_match)
    # 提取八个点对坐标
    Positions = []
    for i in good:
        Positions.append([kp1[i[0].queryIdx].pt,kp2[i[0].trainIdx].pt])
    i=0
    for points in Positions: # 随机颜色重新绘图
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)
        cv.putText(img_match,str(i),(int((points[0])[0]),int((points[0])[1])),cv.FONT_HERSHEY_SIMPLEX,3,(r,g,b),3)
        cv.circle(img_match,(int((points[0])[0]),int((points[0])[1])), 10, (r,g,b), -1)
        cv.circle(img_match,(int((points[1])[0])+4272,int((points[1])[1])), 10, (r,g,b), -1)
        cv.line(img_match,(int((points[0])[0]),int((points[0])[1])),(int((points[1])[0])+4272,int((points[1])[1])),(r,g,b),5,-1)
        i=i+1
    # cv.imwrite("./img/match.png", img_match)
    return Positions

img1 = cv.imread('./img/001.jpg',0)
img2 = cv.imread('./img/002.jpg',0)

# Positions[0][0] Positions[0][1] ~ Positions[7][0] Positions[7][1]
Positions = Find8BestPoints(img1,img2)
# 提取所有坐标为4个列表

def FindNormalizeMat(px,py):
    u = sum(px)/8
    v = sum(py)/8
    s = np.sqrt(2)*8 / sum(np.sqrt((px-u)*(px-u)+(py-v)*(py-v)))
    T = np.array(([s,0,-s*u],[0,s,-s*v],[0,0,1]))
    return T

px1 = np.array(Positions)[:,0][:,0]
py1 = np.array(Positions)[:,0][:,1]
px2 = np.array(Positions)[:,1][:,0]
py2 = np.array(Positions)[:,1][:,1]
# print(px1,py1,px2,py2,Positions)
T1 = FindNormalizeMat(px1,py1)
T2 = FindNormalizeMat(px2,py2)
Q1=(T1@np.row_stack((px1,py1,np.ones((1,8))))).T
Q2=(T2@np.row_stack((px2,py2,np.ones((1,8))))).T
Q=np.array([list(map(lambda x,y:x*y,Q1[:,0],Q2[:,0])),
            list(map(lambda x,y:x*y,Q1[:,1],Q2[:,0])),
            list(map(lambda x,y:x*y,Q1[:,2],Q2[:,0])),
            list(map(lambda x,y:x*y,Q1[:,0],Q2[:,1])),
            list(map(lambda x,y:x*y,Q1[:,1],Q2[:,1])),
            list(map(lambda x,y:x*y,Q1[:,2],Q2[:,1])),
            list(map(lambda x,y:x*y,Q1[:,0],Q2[:,2])),
            list(map(lambda x,y:x*y,Q1[:,1],Q2[:,2])),
            list(map(lambda x,y:x*y,Q1[:,2],Q2[:,2])),
]).T
# print(Q)
U,D,VT = np.linalg.svd(Q)

# 秩2约束
F = VT.T[:, 8].reshape(3, 3) 
U1, D1, VT1 = np.linalg.svd(F) 
F = U1 @ np.diag((D1[0], D1[1], 0)) @ VT1
# 逆归一化
F = T2.T @ F @ T1
print(F)