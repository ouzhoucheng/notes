# DEVKIT-mpc5744p配置can通信
软件：`S32 Design Studio for Power Architecture Version 2.1`
[新建工程,配置与生成代码](https://blog.csdn.net/weixin_46143152/article/details/125563870)

# can协议
[](https://blog.csdn.net/XiaoXiaoPengBo/article/details/116206252)

1. CAN: 控制器局域网络, 异步通信, CAN_H & CAN_L构成一组差分信号线
    ![](https://img-blog.csdnimg.cn/img_convert/cf05a59fda4585094f10283ff2ea0316.png)
2. 闭环总线网络: (ISO11898)标准高速, 短距离闭环网络, 最长40m, 最快1Mbps, 两端有120$\Omega$电阻
    ![](https://img-blog.csdnimg.cn/img_convert/3993c286a7ec510d0bb6cbd003a3518f.png)
3. 开环总线网络: (ISO11519-2)标准低速, 长距离开环网络, 最长1km, 最快125kbps, 两线独立不闭环, 各串2.2$k\Omega$电阻
    ![](https://img-blog.csdnimg.cn/img_convert/8c6ef728dad6d9cb8e3b3dcb7df27969.png)
4. 不对节点编码, 对内容编码, 可挂载多节点, 负载足够理论上数量不限, 可通过中继器增强负载
5. 节点: CAN控制器+CAN收发器 
    ![](https://img-blog.csdnimg.cn/af08402d8cf34d40ae78f9797808060f.png)
    STM32的CAN片上外设即通讯节点的控制器,需要外接收发器(TJA1050)
6. 差分信号: 逻辑1(隐形电平), 逻辑0(显性电平)
    ![](https://img-blog.csdnimg.cn/img_convert/3247c023be2cae8d94ec5d6fe86501d8.png)
7. CAN报文: 对数据, 操作命令, 同步信号进行打包
   1. 数据帧: 传输起始标签+片选标签+控制标签+原始数据段+CRC校验标签+应答标签+传输结束标签
      1. 数据帧: 节点向外传送数据
      2. 遥控帧: 向远端节点请求数据
      3. 错误帧: 向远端节点通知校验错误, 请求重发上一个数据
      4. 过载帧: 通知远端节点:本节点尚未准备好接收
      5. 帧间隔: 将数据帧及遥控帧与前面的帧分离开
   2. 数据帧结构
      1. 帧起始: SOF段, 1位, 
   3. 仲裁: 有两个报文要发送, 总线根据哪个数据包能被传输, 发送报文时先出现隐形电平则失去对总线占有权, 仲裁段值越小, 优先级越高
        ![](https://img-blog.csdnimg.cn/img_convert/7f5fb78e7aab3e2f479507ebcf682631.png)
1. 模式![](https://img-blog.csdnimg.cn/img_convert/e5d6aba5a39f0b0d626d0c82ae105f5e.png)
   1. 正常模式: 可收发
   2. 静默模式: 只能向总线发送隐性位, 监测总线数据
   3. 回环模式: 输出->输入 & 总线, 不能接收总线, 用于自检
   4. 回环静默模式: 自我检查, 不干扰总线
