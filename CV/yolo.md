# yolov3

## 概念

- 人工智能：类脑思考机器
- 机器学习Machine Learning：机器从历史数据学习，实现人工智能的方法
- 深度学习Deep Learning、强化学习：机器学习中的一种技术，更好处理识图、语音、语义

## 常用框架

- Tensorflow：Google旗下的开源软件库，含深度学习的各类标准算法API和数据集等
- Pytorch：Facebook旗下的开源机器学习库，含大量的深度学习标准算法API和数据集等
- Caffe：贾扬清博士开发的深度学习框架
- Paddle-Paddle：百度的深度学习框架
- darknet:YOLO作者JosephRedmon自己写的深度学习框架

在上诉的各种框架内可以实现具体的**目标检测算法**，如R-CNN、Fast RCNN、YOLO、SSD等等。

## YOLOV3

- YOLO: (You only look once)一种实施目标检测算法
- YOLOv3：18年推出的单阶段目标检测算法，快速准确，广泛应用于目标识别、对象分类、定位场景

### 基本原理

目标检测任务需要做的事情是识别和定位。YOLO v3的基本思想是把输入的图像分割成很多个小格子grid cells,接着根据边界框(bounding boxes)和置信度(confidence)来确定定位，同时通过类别概率图(class probability map)确定识别目标类型，最后输出预测的结果。

![](https://img-blog.csdnimg.cn/1588ad2329e340ddaa961562881c922c.png)

## 训练数据集

让YOLO识别指定的目标，需准备一套数据集，对YOLO进行训练，“教会"YOLO认识这些目标。训练结果是生成一个.weight权重文件，识别时，加载权重文件即可识别指定的目标。

### 使用

#### 硬件

- 台式/笔记本，有NVIDIA显卡，装Ubuntu，显卡驱动，编译darknet
- NVIDIA的嵌入式设备

#### 编译

下载`git clone https://github.com/AlexeyAB/darknet`，makefile文件设置，make编译

#### 数据集标注

按PASCAL VOC数据集格式存储数据，制作VOC格式数据集，转换为YOLO标签格式
![区别：白色yolo，红色voc](https://img-blog.csdnimg.cn/2e1ecae18076402b82d741f1ef001420.png)

#### 训练

准备文件，修改参数，预训练权重文件，训练

#### 测试

查看AP值，达到要求可使用(.cfg .weight)