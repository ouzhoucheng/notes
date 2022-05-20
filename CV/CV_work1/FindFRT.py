import cv2 as cv
import random
import numpy as np


# 寻找八对最佳匹配点，参数：原灰度图
def Find8BestPoints(img1,img2):

    # surf算法寻找特征点
    surf = cv.xfeatures2d.SURF_create(11200,4,4,False,False)
    kp1, desc1 = surf.detectAndCompute(img1, None)
    kp2, desc2 = surf.detectAndCompute(img2, None)

    # bf匹配器
    bf = cv.BFMatcher()
    matches = bf.knnMatch(desc1, desc2,k=2)

    # 应用比例测试选择要使用的匹配结果
    good = []
    for m,n in matches:
        if m.distance < 0.6*n.distance:
            good.append([m])
    # print(len(good))

    good = sorted(good, key=lambda x: x[0].distance) # 排序
    # 排除掉重复的点对
    for m in good: 
        for n in good:
            if  n!=m and (m[0].trainIdx == n[0].trainIdx):
                good.remove(n)
    good = good[:8] # 取出八个最优的

    # 匹配作图和保存
    img_match = cv.drawMatchesKnn(img1, kp1, img2, kp2, good, None, flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
    # cv.imwrite("./img/match.png", img_match)

    # 提取八个点对的坐标
    Positions = []
    for i in good:
        Positions.append([kp1[i[0].queryIdx].pt,kp2[i[0].trainIdx].pt])

    # 重新画线标记，确保无错误匹配情况
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
    cv.imwrite("./img/match.png", img_match)

    return Positions


# 求归一化矩阵，参数：坐标列表x,y
def FindNormalizeMat(px,py):

    u = sum(px)/8
    v = sum(py)/8
    s = np.sqrt(2)*8 / sum(np.sqrt((px-u)*(px-u)+(py-v)*(py-v)))
    T = np.array(([s,0,-s*u],[0,s,-s*v],[0,0,1]))
    return T


# 求解基础矩阵，参数：坐标列表px1,py1,px2,py2
def SolveBasisMat(px1,py1,px2,py2):

    # 归一化
    T1 = FindNormalizeMat(px1,py1) 
    T2 = FindNormalizeMat(px2,py2)

    # 归一化坐标
    W1=(T1@np.row_stack((px1,py1,np.ones((1,8))))).T
    W2=(T2@np.row_stack((px2,py2,np.ones((1,8))))).T

    # 奇异值分解求F矩阵
    W=np.array([list(map(lambda x,y:x*y,W1[:,0],W2[:,0])),
                list(map(lambda x,y:x*y,W1[:,1],W2[:,0])),
                list(map(lambda x,y:x*y,W1[:,2],W2[:,0])),
                list(map(lambda x,y:x*y,W1[:,0],W2[:,1])),
                list(map(lambda x,y:x*y,W1[:,1],W2[:,1])),
                list(map(lambda x,y:x*y,W1[:,2],W2[:,1])),
                list(map(lambda x,y:x*y,W1[:,0],W2[:,2])),
                list(map(lambda x,y:x*y,W1[:,1],W2[:,2])),
                list(map(lambda x,y:x*y,W1[:,2],W2[:,2])),
    ]).T
    # print(Q)
    U,D,VT = np.linalg.svd(W)

    # 秩2约束
    F = VT.T[:, 8].reshape(3, 3) 
    U1, D1, VT1 = np.linalg.svd(F) 
    F = U1 @ np.diag((D1[0], D1[1], 0)) @ VT1

    # 逆归一化
    F = T2.T @ F @ T1

    return F


# 求解外参矩阵RT，参数：本质矩阵，用哪一点作验证，8点坐标
def SolveRT(E,which,Parray):

    # 本质矩阵SVD分解
    Ue,De,VTe = np.linalg.svd(E)
    k=(De[1]+De[2])/2
    # 正交矩阵
    W_=np.array(([0,-1,0],[1,0,0],[0,0,1]))
    # 反对称阵
    Z=np.dot(np.diag([1,1,0]),W_)

    # 求解RT，分别由两种情况
    R1 = Ue @ W_.T @ VTe
    R2 = Ue @ W_ @ VTe
    T1 = Ue @ np.diag([0,0,1])
    T2 = Ue @ np.diag([0,0,-1])

    # 四种匹配方式
    P11 = K @ np.append(R1,np.array([T1[:,2]]).T,axis=1)
    P12 = K @ np.append(R1,np.array([T2[:,2]]).T,axis=1)
    P21 = K @ np.append(R2,np.array([T1[:,2]]).T,axis=1)
    P22 = K @ np.append(R2,np.array([T2[:,2]]).T,axis=1)
    P0 = K @ np.append(np.eye(3),[[0],[0],[0]],axis=1)

    # 三角化得到坐标
    out1 = cv.triangulatePoints(P0, P11, Parray[:,0][which], Parray[:,1][which])
    out2 = cv.triangulatePoints(P0, P12, Parray[:,0][which], Parray[:,1][which])
    out3 = cv.triangulatePoints(P0, P21, Parray[:,0][which], Parray[:,1][which])
    out4 = cv.triangulatePoints(P0, P22, Parray[:,0][which], Parray[:,1][which])

    # z坐标为正的RT矩阵是所求矩阵
    if out1[2]>0:
        return R1,T1,1
    elif out2[2]>0:
        return R1,T2,2
    elif out3[2]>0:
        return R2,T1,3
    elif out4[2]>0:
        return R2,T2,4
    else:
        return 0,0,0


if __name__ == '__main__': 
    img1 = cv.imread('./img/001.jpg',0)
    img2 = cv.imread('./img/002.jpg',0)
    # 内参数矩阵
    K = np.array([[4678.430192307692, 0, 2136], [0, 4678.430192307692, 1424], [0, 0, 1]])

    # 寻找八对最佳匹配点 Positions[0][0] Positions[0][1] ~ Positions[7][0] Positions[7][1]
    Positions = np.array(Find8BestPoints(img1,img2))

    # 提取所有坐标为4个列表
    # Positions = np.array(Positions)
    px1 = Positions[:,0][:,0]
    py1 = Positions[:,0][:,1]
    px2 = Positions[:,1][:,0]
    py2 = Positions[:,1][:,1]

    # 求解基础矩阵
    F = SolveBasisMat(px1,py1,px2,py2)
    print("基础矩阵\n",F)

    P1 = np.row_stack((px1, py1, np.ones((1, 8), dtype=int))) 
    P2 = np.row_stack((px2, py2, np.ones((1, 8), dtype=int)))
    testF = P2.T @ F @ P1 
    test = []
    for i in range(8): 
        test.append(testF[i][i]) 
    print("基础矩阵测试\n",test)

    # 求解本质矩阵
    E=np.dot(np.dot(K.T,F),K)

    # 用所有点对作验证
    # for i in np.linspace(0,7,8):
    #     R,T,whichRT = SolveRT(E,int(i))
    #     print(whichRT)

    # 求解外参矩阵RT
    R,T,whichRT = SolveRT(E,0,Positions)
    print("外参旋转\n",R)
    print("外参平移\n",T)