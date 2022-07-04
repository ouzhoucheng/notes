# DEVKIT-mpc5744p配置etimer中断
软件：`S32 Design Studio for Power Architecture Version 2.1`
[新建工程,配置与生成代码](https://blog.csdn.net/weixin_46143152/article/details/125563870)
1. `Components`双击添加`eTimer`模块, 改名
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/6adb31077ce643ffa09886309fc6ff2e.png)
2. 改成上下沿计数模式
   ![在这里插入图片描述](https://img-blog.csdnimg.cn/c6b33f0a679a467baa1633e62cfec0a5.png)
3. 主要时钟输入改为分频输入
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/93d36e19d1674d7abf24300d46b110d0.png)
4. 双击ClockMan1, 可以看到eTimer1的时钟源也是MOTC, 下边可以改MOTC的时钟源, 最后一个选项卡可以看eTimer具体时钟频率
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/cb3f7c39fd274b19ad8441e3ee7d84d6.png)
5. 保存,生成代码
6. 在`main.c`里添加业务代码
    ```C++
    // 中断函数
    void ETIMER0_Ch0_IRQHandler(void)
    {
    #define CNT_TIMEOUT     2
        static uint32_t cnt=CNT_TIMEOUT;
        if(--cnt==0)
        {
            /* RED LED toggle */
            PINS_DRV_TogglePins(PTC,(1 << 12));
            cnt=CNT_TIMEOUT;
        }
        /* need to reset flags */
        ETIMER_DRV_ClearInterruptStatus(0,ETIMER_CH_IRQ_FLAGS_TOF,0);
    }
    // 定时器中断的初始化
    /* setup ETIMERx channels */
    ETIMER_DRV_Init(INST_ETIMER0);
    ETIMER_DRV_InitChannel(INST_ETIMER0, 0, &eTimer0_etimer_user_channel_config0);
    /* Enable ETIMER0 channel 0 interrupt and install an ISR */
    IRQn_Type etimer0_ch0_irq_id = ETIMER_DRV_GetInterruptNumber(0,ETIMER_IRQ_CH0);
    INT_SYS_InstallHandler(etimer0_ch0_irq_id, &ETIMER0_Ch0_IRQHandler, (isr_t*) 0);
    INT_SYS_EnableIRQ(etimer0_ch0_irq_id);
    /* enable interrupts */
    ETIMER_DRV_EnableInterruptSource(INST_ETIMER0,ETIMER_CH_IRQ_SOURCE_TOFIE,0);
    /* start channel operation */
    ETIMER_DRV_StartTimerChannels(INST_ETIMER0, (ETIMER_ENABLE_CH0));
    ```
7. 编译,Debug

 