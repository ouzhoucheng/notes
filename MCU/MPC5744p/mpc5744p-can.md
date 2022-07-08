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

# 配置
1. 在`component library`中双击添加`can pal`模块
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/96fd6906220d4114a0bc35f921ceab31.png)
2. 再在`components`中双击这个模块，进入配置界面，完成配置
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/6bb1c5d8f8734a1da44d4ce28b66321d.png)
3. `ctrl+s`保存， 生成代码
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/340082fcf6c944f1bd18002fe94d7b1e.png)
4. 添加发送邮箱和接受邮箱、报文id
    ```C++
    #define TX_MAILBOX  (1UL)
    #define TX_MSG_ID   (0x300)
    #define RX_MAILBOX  (0UL)
    #define RX_MSG_ID   (0x301)
    ```
5. 可以[开启定时器中断](https://blog.csdn.net/weixin_46143152/article/details/125605983), 中断发送, 也可以在while延时循环发送
    ```C++
    CAN_Init(&can_pal1_instance, &can_pal1_Config0);    //  初始化 
    ```

    ```C++
    uint8_t CanCount = 0;   // can计数
    uint8_t SendBuff[8] = {1,2,3,4,5,6,7,8};    //  发送内容
    uint8_t ReceiveCount = 0;   //  接收计数, 可忽略

    /*** 发送 ***/
    SendBuff[7] = CanCount; //  末位自加1
    can_buff_config_t buffCfg =  {
            .enableFD = false,
            .enableBRS = false,
            .fdPadding = 0U,
            .idType = CAN_MSG_ID_STD,
            .isRemote = false
    };
    /* Configure TX buffer with index TX_MAILBOX*/
    CAN_ConfigTxBuff(&can_pal1_instance, TX_MAILBOX, &buffCfg);
    /* Prepare message to be sent */
    can_message_t message = {
        .cs = 0U,
        .id = 0x300,    //  发送id, 即TX_MSG_ID
        .data[0] = SendBuff[0],
        .data[1] = SendBuff[1],
        .data[2] = SendBuff[2],
        .data[3] = SendBuff[3],
        .data[4] = SendBuff[4],
        .data[5] = SendBuff[5],
        .data[6] = SendBuff[6],
        .data[7] = SendBuff[7],
        .length = 8U    //  发送数据长度
    };
    CanCount++; //  末位自加1, 可不加
    /* Send the information via CAN */
    CAN_Send(&can_pal1_instance, TX_MAILBOX, &message); //  发送
    ```

6. 在`while`中循环接收
    ```C++
    while(1)
    {
        can_message_t recvMsg;
        /* Start receiving data in RX_MAILBOX. */
        CAN_Receive(&can_pal1_instance, RX_MAILBOX, &recvMsg);
        /* Wait until the previous FlexCAN receive is completed */
        while(CAN_GetTransferStatus(&can_pal1_instance, RX_MAILBOX) == STATUS_BUSY);
        /* Check the received message ID and payload */
        if(recvMsg.id == 0x301)
        {
            PINS_DRV_TogglePins(PTC,(1 << 13));
            SendBuff[ReceiveCount] = recvMsg.data[0];
            ReceiveCount++;
            if(ReceiveCount > 6)
            {
                ReceiveCount = 0;
            }
        }
    }
    ```
7. 编译, debug