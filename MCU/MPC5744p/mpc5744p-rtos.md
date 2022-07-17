# DEVKIT-mpc5744p配置rtos
软件：`S32 Design Studio for Power Architecture Version 2.1`
[新建工程,配置与生成代码](https://blog.csdn.net/weixin_46143152/article/details/125563870)
ref: 野火 《FreeRTOS 内核实现与应用开发实战—基于STM32》
可在野火大学堂下载 或 [下载链接](https://gitee.com/Wind_to_valley/notes/blob/master/MCU/RTOS/%E3%80%8AFreeRTOS%20%E5%86%85%E6%A0%B8%E5%AE%9E%E7%8E%B0%E4%B8%8E%E5%BA%94%E7%94%A8%E5%BC%80%E5%8F%91%E5%AE%9E%E6%88%98%E2%80%94%E5%9F%BA%E4%BA%8ESTM32%E3%80%8B.pdf)
# 配置
1. 在`component library`中双击添加`rtos`模块
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/00e77aee915747f183de709e33524b27.png)
2. 再在`components`中双击这个模块，进入配置界面，完成配置
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/c6700c48c95e44f3b64cf8bc00bc71d3.png)
3. 配置3个led接口为输出
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/7f5a161d586847f9829874f5570c823d.png)
4. `ctrl+s`保存， 生成代码
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/9ffdca87998d4ec7b1d27a8aecdc3626.png)
5. 添加`Task/task_start.c task_start.h task_led.c task_led.h` (可在别的地方创建， 拖进工程文件夹)
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/5a855bf20463454a82a34d91635ca0be.png)
6. 在`main.c`中添加包含头文件, 时钟, 引脚, rtos初始化
    ```C++
    #include "task_start.h"

        CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
        CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
        PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
        RTOS_Start();
    ```
7. `task_start.h`
    ```C
    #ifndef TASK_START_H
    #define TASK_START_H
    #include "FREERTOS.h"
    #include "task.h"
    #include "FREERTOS.h"
    #include "queue.h"
    #include "semphr.h"
    #include "task_led.h"
    #include "task_swt.h"
    #include "Cpu.h"
    extern void StartTask(void * pvParameters);
    extern void RTOS_Start(void);
    #endif
    ```
8. `task_start.c`
    ```C++
    #include "FREERTOS.h"
    #include "queue.h"
    #include "semphr.h"
    #include "task_start.h"
    TaskHandle_t StartTask_Handle;
    void StartTask(void * pvParameters) //  初始化3个led任务
    {
        taskENTER_CRITICAL();               					//进入临界区

        FCCU_DRV_Init(INST_FCCU1, fccu1_Control0, fccu1_NcfConfig0);
        SWT_DRV_Init(INST_SWT1, &swt1_Config0);
        PINS_DRV_ClearPins(PTC,(1 << 11));
        PINS_DRV_SetPins(PTC,(1 << 12));
        xTaskCreate(vLEDTaskG,(const char * const)"vLEDTaskG", configMINIMAL_STACK_SIZE, (void*)0, 2, NULL);
        xTaskCreate(vLEDTaskB,(const char * const)"vLEDTaskB", configMINIMAL_STACK_SIZE, (void*)0, 2, NULL);
        taskEXIT_CRITICAL();                //退出临界区

        vTaskDelay(1000);

        taskENTER_CRITICAL();               					//进入临界区
        xTaskCreate(vLEDTaskR,(const char * const)"vLEDTaskR", configMINIMAL_STACK_SIZE, (void*)0, 2, NULL);
        PINS_DRV_ClearPins(PTC,(1 << 13));
        xTaskCreate(vSwtTask,(const char * const)"vSwtTask", configMINIMAL_STACK_SIZE, (void*)0, 4, NULL);

        vTaskSuspend(StartTask_Handle);      //初始化完成以后挂起开始任务
        taskEXIT_CRITICAL();                //退出临界区
    }

    void RTOS_Start(void)
    {	
        xTaskCreate(StartTask, ( const char * const )"Start Task", 300, NULL, 10,&StartTask_Handle);	
        
        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }
    ```
9.  `task_led.h`
    ```C++
    #ifndef TASK_LED_H
    #define TASK_LED_H
    #include "FREERTOS.h"
    #include "task.h"
    #include "FREERTOS.h"
    #include "queue.h"
    #include "semphr.h"
    #include "Cpu.h"
    extern void vLEDTaskR( void *pvParameters);
    extern void vLEDTaskG( void *pvParameters);
    extern void vLEDTaskB( void *pvParameters);
    #endif
    ```
10. `task_led.c`
    ```C++
    #include "task_led.h"
    void vLEDTaskR( void *pvParameters)
    {
    while (1)
    {
        vTaskDelay( 1500);
        PINS_DRV_TogglePins(PTC, (1<<11));
    }
    }
    void vLEDTaskG( void *pvParameters)
    {
    while (1)
    {
        vTaskDelay( 500);
        PINS_DRV_TogglePins(PTC, (1<<12));
    }
    }
    void vLEDTaskB( void *pvParameters)
    {
    while (1)
    {
        vTaskDelay( 1500);
        PINS_DRV_TogglePins(PTC, (1<<13));
    }
    }
    ```
11. 编译, debug
12. 这里实现了一个rgb交替闪烁, 显示六种颜色的功能
    ![在这里插入图片描述](https://img-blog.csdnimg.cn/3b09f583463044e68706f4e9116164e0.png)
