# freeRTOS

![源码下载](https://www.freertos.org/a00104.html)

# 文件
FreeRTOS
- License: 许可信息
- Source: 源码
  - include: 头文件
  - portable: 软件到硬件驱动
    - RVDS
      - ARM_CM3: 移植所需
FreeRTOS-Plus: 增加的功能代码

# 移植

```C++
xTaskCreate((TaskFunction_t )start_task, //任务函数
            (const char* )"start_task", //任务名称
            (uint16_t )START_STK_SIZE, //任务堆栈大小
            (void* )NULL, //传递给任务函数的参数
            (UBaseType_t )START_TASK_PRIO, //任务优先级
            (TaskHandle_t* )&StartTask_Handler); //任务句柄 
            vTaskStartScheduler(); //开启任务调度
```