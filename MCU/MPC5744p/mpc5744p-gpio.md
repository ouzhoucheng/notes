# DEVKIT-mpc5744p配置gpio
软件：`S32 Design Studio for Power Architecture Version 2.1`
1. 新建工程
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/5eccbcf135f44803a44b75a32e497636.png)
2. 搜索选择芯片
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/7cad0b8f065744778047f84b24fcafbb.png)
3. 选择SDK，新建
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/76803413c71345dba33d080857eb5d3a.png)
4. 先编译一下, 双击`Components`的`pin_mux`打开引脚配置, 没有这个窗口的话: Window->Show View->Other->Processor Expert
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/77f32808600447eabfd5dfac5ea0d545.png)
5. 配置LED所在引脚PC13为输出
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/ce2068c39ba742e292367f52edb6c20b.png)
6. ctrl+s保存, 点击生成代码
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/e754580fbe754e6a92bbfcabe9f93795.png)
7. 添加初始化和业务代码
    ```C++
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr); // 初始化所有引脚
    while(1)
    {
        /* Insert a small delay to make the blinking visible */
        delay(720000);
        /* Toggle output value LED1 */
        PINS_DRV_TogglePins(PTC, (1<<13));  // PTC13闪烁
    }
    ```
    在main函数前加一个延时函数
    ```C
    void delay(volatile int cycles)
    {
        /* Delay function - do nothing for a number of cycles */
        while(cycles--);
    }
    ```
8. 编译, Debug, 成功!
9. 在`SDK/platform/drivers/src/pins/siul2/pins_siul2_driver.c`中可以找到其他gpio置位,复位等函数



