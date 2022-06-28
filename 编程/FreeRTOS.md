# FreeRTOS
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

1. 原工程添加`FreeRTOS`文件夹
2. 将`\FreeRTOSv202112.00\FreeRTOS\Source`的文件拷贝进去
3. `portable`中删剩下`Keil`, `MemMang`, `RVDS`
4. 在MDK工程添加分组和文件
    ```md
    |--FreeRTOS_CORE
        |--croutine.c
        |--event_groups.c
        |--list.c
        |--queue.c
        |--tasks.c
        |--timers.c
    |--FreeRTOS_PORTABLE
        |--port.c
        |--heap_4.c
    ```
5. 添加头文件路径`\FreeRTOS\include`和`\FreeRTOS\portable\RVDS\ARM_CM3`
6. 从官方历程`\FreeRTOSv202112.00\FreeRTOS\Demo\CORTEX_STM32F103_Keil`中找到文件`FreeRTOSConfig.h`,拷贝到工程目录的`\FreeRTOS\include`中
7. 编译