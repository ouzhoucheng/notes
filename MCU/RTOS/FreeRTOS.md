# 1.FreeRTOS
![源码下载](https://www.freertos.org/a00104.html)

## 1.1.文件
FreeRTOS
- License: 许可信息
- Source: 源码
  - include: 头文件
  - portable: 软件到硬件驱动
    - RVDS
      - ARM_CM3: 移植所需
FreeRTOS-Plus: 增加的功能代码
## 1.2.移植

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

## 1.3.FreeRTOS的编程风格

### 1.3.1.数据类型
见`portmacro.h`
- portCHAR: char
- portSHORT: short
- portTickType: 定义系统时基计数器值&阻塞时间
  - unsigned short int: `FreeRTOSConfig.h: configUSE_16_BIT_TICKS=1` 为16位 
  - unsigned int: `FreeRTOSConfig.h: configUSE_16_BIT_TICKS=01` 为32位 
- portBASE_TYPE: long 定义函数返回值/布尔类型: 32/16/8bit处理器--32/16/8bit数据类型

### 1.3.2.变量名
前缀:类型
- c: char型
- s: short型
- l: long型
- x: portBASE_TYPE型, 数据结构, 任务句柄, 队列句柄
- u: 无符号型 uc: 无符号char
- p: 指针变量 pc: char指针

### 1.3.3.函数名
前缀:哪个头文件定义
- port: portable.h
- task: task.h
- pd: projdefs.h
- config: FreeRTOSConfig.h
- err: projdefs.h
通用宏定义
- pdTRUE: 1
- pdFALSE: 0
- pdPASS: 1
- pdFAIL: 0

## 1.4.裸机&多任务系统
![在这里插入图片描述](https://img-blog.csdnimg.cn/f36e094a0c9344338279433845f9f11e.png)

## 1.5.数据结构
### 1.5.1.链表
通过节点将离散的数据链接到一起,组成一个表,常规操作:插入&删除, 通常人为规定一个根节点(生产者),还有个节点计数器.
![在这里插入图片描述](https://img-blog.csdnimg.cn/064451bb77d4439cb5af8580e051d1e8.png)


含有n个节点,前一节点有一个箭头指向后一节点,首尾相连. 节点是自定义类型的数据结构,本身含一个结点指针, 指向后一节点, 还携带一些私有信息(单个数据,数组,指针,结构体等).
![单向链表](https://img-blog.csdnimg.cn/84a898868ffb407ca98a6c39a1b05989.png)

通常在节点中只包含一个用于指向下一个节点的指针,通过链表存储的数据内嵌一个节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/2139e729c01946b7ac82e6ab4fbc9a85.png)

### 1.5.2.双向链表
两个节点指针,分别指向前后两个节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/a2f60c62072d4e46a1925fa76a00150e.png)

链表&数组![在这里插入图片描述](https://img-blog.csdnimg.cn/4ffc8478db0645bfb514fdf45149c44e.png)
- 数组:开辟一段连续内存来存储数据,有起始结束地址
- 链表:一个圈,通常人为规定一个根节点,对节点插入/删除来实现存取数据

### 1.5.3.FreeRTOS的链表
链表节点数据结构定义
```C++
struct xLIST_ITEM
{
    TickType_t xItemValue; // 辅助值:帮节点做顺序排列
    struct xLIST_ITEM * pxNext; // 指向链表下一个节点
    struct xLIST_ITEM * pxPrevious; // 指向链表前一个节点
    void * pvOwner; // 指向拥有该节点的内核对象(TCB)
    void * pvContainer; // 指向该节点所在链表
};
typedef struct xLIST_ITEM ListItem_t; // 节点数据类型重定义
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/0d0eabcbaef845c587ba2f513820f484.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/37f3929b80f94b2295e2ce6b9ac4b92b.png)



# 2.任务

# 2.1.创建任务

- 任务栈: 多任务系统中,每个任务分配独立的栈空间(RAM中预定义的全局数组/动态分配的一段内存空间)
- 任务控制块: 存有任务栈指针,名称,形参等所有信息
- 任务创建函数: 任务栈,函数实体,控制块联系起来
  - 动态创建: 任务控制块,栈的内存是创建任务时动态分配,删除时内存释放
  - 静态创建: 控制块,栈的内存事先定义好,为静态内存,,删除时不释放

```C++
TaskHandle_t Task1_Handle;  //  任务句柄
#define TASK1_STACK_SIZE                    128 //  任务栈大小
StackType_t Task1Stack[TASK1_STACK_SIZE]; //  任务栈
TCB_t Task1TCB; //  任务控制块
```

```C++
Task1_Handle = xTaskCreateStatic( (TaskFunction_t)Task1_Entry,   /* 任务入口 */
                                  (char *)"Task1",               /* 任务名称，字符串形式 */
                                  (uint32_t)TASK1_STACK_SIZE ,   /* 任务栈大小，单位为字 */
                                  (void *) NULL,                 /* 任务形参 */
                                  (UBaseType_t) 2,               /* 任务优先级，数值越大，优先级越高 */
                                  (StackType_t *)Task1Stack,     /* 任务栈起始地址 */
                                  (TCB_t *)&Task1TCB );   
```

# 临界段保护
- 临界段： 执行时不能被中断的代码段 (操作全局变量)

# 空闲任务与阻塞延时
- RTOS的延时: 阻塞延时
- 空闲任务: 启动调度器时创建的优先级最低的任务
- 阻塞延时: 调用时任务被剥离cpu使用权, 进入阻塞状态, 可去执行其他任务(若也阻塞, 运行空闲任务)

`FreeRTOSConfig.h`决定`SysTick`的中断周期
```C++
#define configCPU_CLOCK_HZ			( ( unsigned long ) 25000000 )	
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )

vTaskDelay( 1 );
```

# 多优先级
- 就绪列表`pxReadyTasksLists[configMAX_PRIORITIES]`数组存储就绪任务的TCB(的xStateListItem节点), 数组下标越小, 任务优先级越低
- 创建任务时根据任务优先级将任务插入到就绪列表不同位置, 同优先级插入同一条链表
- `pxCurrenTCB`: 全局TCB指针, 任务切换时指向优先级最高的就绪任务TCB
- `taskSELECT_HIGHEST_PRIORITY_TASK`

```C++
taskSELECT_HIGHEST_PRIORITY_TASK() // 寻找优先级最高的就绪任务 (更新`uxTopReadyPriority`和`pxCurrentTCB`的值)
taskRESET_READY_PRIORITY( uxPriority )  //  将变量uxTopReadyPriority某个位清0
// 实现任务延时列表后, 任务非就绪时, 清零uxTopReadyPriority对应位, 将任务从就绪列表删除
```

# 延时列表
- 任务需要延时时,将任务挂起, 从就绪列表删除, 插入延时列表, 更新下一个任务解锁时刻变量`xNextTaskUnblockTime`
- `xNextTaskUnblockTime` = 时基计数器值`xTickCount` + 任务延时`xTickToDelay`
  - 每次时基中断来, `比较xTickCount` `xNextTaskUnblockTime`

# 支持时间片
- 同一优先级下多个任务, 每个任务轮流享有相同CPU时间

# 移植
- 配置`FreeRTOSConfig.h`
  ```C++
  #include "stm32f10x.h"  //  
  #include "bsp_usart.h"  //  断言操作需要打印

  #define xPortPendSVHandler PendSV_Handler // 中断服务函数配置相关
  #define vPortSVCHandler SVC_Handler
  ```
- 配置`stm32f10x_it.c`
  ```C++
  //systick 中断服务函数 
  void SysTick_Handler(void) 
  { 
  #if (INCLUDE_xTaskGetSchedulerState == 1 ) 
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
  #endif /* INCLUDE_xTaskGetSchedulerState */ 
          xPortSysTickHandler();
  #if (INCLUDE_xTaskGetSchedulerState == 1 ) 
      }
  #endif /* INCLUDE_xTaskGetSchedulerState */
  }
  ```

# 创建任务
1. 板级初始化
1. 定义任务函数
1. 实现空闲任务, 定时器任务堆栈函数
2. 定义任务栈
3. 定义任务控制块
4. 创建静态任务
5. 启动任务

# 任务管理
- 任务: 竞争系统资源的最小运行单元
  - FreeRTOS的任务可认为是一系列独立任务集合, 每个任务在自己环境中运行, 同一时刻一个任务运行(调度器决定启动停止, 切入切出保存上下文环境), 宏观上每个任务都在执行, 系统可用SRAM决定能运行多少任务
  - 抢占式调度机制: 高优先级任务可打断低优先级任务, 低优先级任务必须在高优先级任务阻塞/结束才能得到调度
- 任务调度器: 基于优先级的全抢占式调度
  - 中断处理函数, 调度器上锁部分, 禁止中断 不可抢占, 其他部分可抢占
  - 同优先级采用时间片轮转方式调度, 最大可能保证高优先级任务得以运行
- 任务状态迁移
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/4dec8b416fb8486eacb3b8e91b2ad558.png)
  - 就绪态: 任务准备就绪, 等调度器进行调度, 有更高优先级任务创建/恢复, 原运行任务->就绪态
  - 运行态: 任务切换时, 就绪列表的最高优先级任务被执行, 选择运行的永远是最高优先级的就绪态任务
  - 阻塞态: 运行任务发生阻塞(被挂起,延时,等待信号量), 任务退出就绪列表, 切换任务
  - 挂起态: 通过`vTaskSuspend()`将处于任何状态的任务挂起, 挂起后无cpu使用权, 不参与调度, 用`vTaskResume()`解除
    - `vTaskSuspend()`任务挂起函数
    - `vTaskResume()`任务恢复函数
    - `xTaskResumeFromISR()`恢复被挂起的任务(中断)
    - `vTaskSuspendAll()`挂起调度器, 相当于挂起所有任务
    - `xTaskResumeAll()`恢复调度器, 用多少次挂起要用多少次恢复
- 函数
  - `vTaskDelete()`任务删除函数
  - `vTaskDelay()`相对任务延时函数, 不适用于周期性执行任务的场合
  - `vTaskDelayUntil()`绝对延时函数, 实现固定频率定期执行任务

# 消息队列
- 消息队列: 常用于任务间通信的数据结构
  - 可在任务与任务间中断和任务间传递信息, 实现任务接受来自其他任务/中断的不定长消息
  - 
