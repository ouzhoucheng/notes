# DEVKIT-mpc5744p配置flexPWM
软件：`S32 Design Studio for Power Architecture Version 2.1`
[新建工程,配置与生成代码](https://blog.csdn.net/weixin_46143152/article/details/125563870)
1. 双击添加flexpwm模块
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/cb5ff5fcfee348019fec72c45d96ba30.png)
2. 选取pwm的输出io口
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/68ba41c3438148f0aa3de8264f19846b.png)
3. 双击`Components/clockMan1`,配置时钟
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/6e49f15ac7ac4af09f45c6dfbd2be3f4.png)
4. 在`Clock Values Summary`下滑可以看到FlexPWM1的时钟频率
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/24213093389441b1895416a80e9a598b.png)
5. FlexPWM对应时钟是MOTC时钟, 在`settings`处可配置MOTC的时钟源和分频, `setting`旁边的选项卡也可以单独配置时钟源的倍频和分频, 这里选择了`IRCOSC/16`, 再到`Clock Values Summary`可看到FlexPWM模块的频率变成了1MHz
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/ca83754f3ae5480cafd44f92f18eac63.png)
6. 双击`Components/flexpwm`模块, all-选择Device`FlexPWM_1`
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/bc938dd7ce904758998553fe6129d3d0.png)
7. 输入周期1000, 则最后pwm频率为 1MHz/1000=1kHz
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/3fef39523ffd4d61aa67ad26331a1cbd.png)
8. 使能输出口
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/e62b0bd93cea437ba54e443605a8b069.png)
9. 保存,生成代码
10. 在`main.c`添加业务代码
    ```C++
    #define STEP 100
    #define MAX_VAL 1000 // 周期

    uint16_t dutyCycle = MAX_VAL;
    bool increaseDutyCycle = false;
    // 初始化pwm1模块0
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
    FLEXPWM_DRV_SetupPwm(INST_FLEXPWM1, 0U, &flexPWM1_flexpwm_module_setup_t0, &flexPWM1_flexpwm_module_signal_setup_t0);
    FLEXPWM_DRV_CounterStart(INST_FLEXPWM1, 0U);
    FLEXPWM_DRV_UpdatePulseWidth(INST_FLEXPWM1, 0U, 300, 0UL, FlexPwmEdgeAligned);
    FLEXPWM_DRV_LoadCommands(INST_FLEXPWM1, (1UL << 0));
    // 
    while(1)
    {
        /* Insert a small delay to make the blinking visible */
        delay(7200000);
        /* Toggle output value LED1 */
        PINS_DRV_TogglePins(PTC, (1<<12));

        if (increaseDutyCycle == false)
        {
            if (dutyCycle < STEP)
                increaseDutyCycle = true;
            else
                dutyCycle -= STEP;
        }
        else
        {
            if (dutyCycle > MAX_VAL - STEP)
                increaseDutyCycle = false;
            else
                dutyCycle += STEP;
        }
        FLEXPWM_DRV_UpdatePulseWidth(INST_FLEXPWM1, 0U, dutyCycle, 0UL, FlexPwmEdgeAligned);
        FLEXPWM_DRV_LoadCommands(INST_FLEXPWM1, (1UL << 0));
    }
    ```
11. 编译,调试
