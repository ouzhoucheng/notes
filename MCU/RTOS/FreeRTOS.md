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
  - 任务可从队列读取消息, 空消息时该任务阻塞
    - 阻塞任务时间`xTicksToWait`, 若队列空, 任务保持阻塞态以等待队列数据有效
    - 有新消息: 唤醒处理消息
    - 超过阻塞时间: 任务就绪
  - 先进先出FIFO
- 特性
  - 消息支持先进先出排队, 异步读写工作方式
  - 读写队列支持超时机制
  - 支持后进先出排队LIFO
  - 允许不同长度任意类型消息
  - 一任务从任意队列接受发送消息
  - 多任务从同一队列接受发送消息
  - 队列使用结束可删除
- 运作机制
  - 创建队列: 分配内存空间 = 单个空间大小 x 消息队列长度
    - 头指针pcHead, 尾指针pcTail, 消息大小uxItemSize, 队列长度uxLength
  - 初始化: 队列为空
  - 发送消息
    - 队列未满/允许覆盖: 拷贝到队列队尾
    - 否: 阻塞
    - 其他任务读入数据/超过阻塞时间 ->就绪态
  - 紧急消息
    - 队列队头
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/1f13684ce3ea4919867758dc542d127b.png)
- 阻塞机制
  - 无消息: 不等, 等一段时间, 持续等
  - 队列满: 入队任务阻塞 
  - 多任务阻塞一个队列: 优先级高获得访问权
- 函数
  - `xQueueCreate()`创建消息队列 分配RAM:队列状态+存储区域
  - `vQueueDelete()`消息队列删除函数
  - 发送
    - `xQueueSend()`向队列尾部发送一个队列消息(不能被中断调用)
    - `xQueueSendFromISR()`中断向队列尾部发消息
    - `xQueueSendToFront()`向队列队首发一个消息
    - `xQueueSendToFrontFromISR()`中断向队列队首发一个消息
    - `xQueueGenericSend()`通用消息队列发送函数
    - `xQueueGenericSendFromISR()`中断通用消息队列发送
  - 接收
    - `xQueueReceive()`接收消息, 将消息从队列删除
    - `xQueueReceiveFromISR()`中断接收消息, 将消息从队列删除, 无阻塞
    - `xQueuePeed()`接收消息, 不删除消息
    - `xQueuePeedFromISR()`中断接收消息, 不删除消息, 无阻塞
    - `xQueueGenericReceive()`通用任务从队列读取消息
- 注意
  - 先创建消息队列, 后使用读写函数
  - 先进先出FIFO / 后进先出LIFO
  - 定义存储读取数据处 > 消息大小
  - 收发以拷贝进行, 大消息可用地址

# 信号量
- 信号量
  - 实现任务间通信的机制, 同步/临界资源的互斥访问, 协助一组相互竞争的任务访问临界资源
  - 非负整数, 获取它的任务将该数-1, ->0所有获取它的任务阻塞
  - 0: 没有积累下来的释放信号量操作, 可能在信号量上阻塞的任务
  - +: 1/多个释放信号量操作
- 二值信号量
  - 临界资源访问/同步
  - 与互斥信号量类似但无优先级继承机制(二值量偏向同步, 互斥量偏向临界资源访问)
  - 同步
    - 信号量创建后置空, 任务1获取信号量->阻塞, 任务2在某条件发生, 释放信号量, 任务1->就绪, 若优先级高即切换: 实现同步 (中断同理)
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/8dd44b07718a461d9593beee4c09f282.png)
- 计数信号量
  - 长度>1的队列, 事件计数&资源管理
  - 事件发生, 任务/中断释放一个信号量(+1), 事件被处理, 取走信号量(-1)
  - 计数值表示系统中可用资源数
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/f7835d9168ef41969fb22ec4882de085.png)
- 互斥信号量
  - 特殊的二值信号量, 优先级继承机制, 保护临界资源, 创建后满
  - 使用临界资源(任意时刻只能被一个任务访问)时, 先获取互斥信号量, 使之空, 其他需要使用资源的任务进入阻塞
- 递归信号量
  - 已获取递归互斥量的任务可重复获取该递归互斥量, 该任务有递归信号量所有权, 获取几次要返还几次, 此前递归互斥量无效, 其他任务无法获取
- 信号量控制块
  - `semphr.h` 结构体与消息队列结构体一致
  - `uxMessagesWaiting`有效信号量个数
    - 二值/互斥信号量 1:有可用信号量 0:无
    - 计数信号量: 可用信号量数
  - `uxLength`队列长度
    - 二值/互斥 最大=1
    - 计数: 最大信号量数
- 函数
  - `xSemaphoreCreateBinary()`创建二值信号量(空), 要先释放才能获取
  - `xSemaphoreCreateCounting()`创建计数信号量
  - `vSemaphoreDelete()`删除一个信号量
  - `xSemaphoreTake()`任务获取信号量
  - `xSemaphoreTakeFromISR()`任务获取信号量
  - `xSemaphoreGive()`任务释放信号量
  - `xSemaphoreGiveFromISR()`中断释放信号量

# 互斥量
- 互斥信号量, 特殊二值信号量, 支持互斥量所有权, 递归访问及防止优先级翻转的特性, 实现对临界资源独占式处理
  - 状态: 开锁 / 闭锁
  - 一任务持有互斥量, 其他任务不能再对互斥量开锁 / 持有, 持有互斥量的任务可再次获得该锁不被挂起(递归访问)
- 优先级继承机制
  - 某资源受互斥量保护, 被1低优先级任务使用, 高优先级申请不到进入阻塞态, 则持有互斥量任务的优先级临时+->高优先级任务
- 运作机制 ![在这里插入图片描述](https://img-blog.csdnimg.cn/34ec10daed004789907ec286a7511b95.png)
- 函数
  - `xSemaphoreCreateMutex()`创建互斥量
  - `prvInitialiseMutex()`初始化互斥量
  - `xSemaphoreCreateRecursiveMutex()`创建递归互斥量
  - `vSemaphoreDelete()`删除互斥量
  - `xSemaphoreTake()`获取互斥量
  - `xSemaphoreTakeRecursive()`获取递归互斥量, 需先创建
  - `xSemaphoreGive()`释放互斥量
  - `xSemaphoreGiveRecursive()`释放递归互斥量

# 事件
- 一种实现任务间通信的机制, 主用于实现多任务间同步, 不传输数据
  - 只能实现事件类型通信, 无数据传输, 可实现一对多, 多对多同步, 一个任务可等待多个事件发生 / 任一时间发生时唤醒任务处理事件 / 几个事件发生后才唤醒任务处理事件
  - 存储于`EventBits_t`类型变量(少量RAM保存事件组状态)
  - 任务通过逻辑 与 / 或 与多个事件建立关联 -> 事件组
    - 与: 关联型同步, 任务感兴趣的若干事件都发生才唤醒
    - 或: 独立型同步, 任务感兴趣的任一时间发生即唤醒
- 特点
  - 事件相互独立, 只与任务关联, 32位时间集合`EventBits_t`24位表示事件: 0:未发生 1:已发生 共24种事件类型
  - 仅用于同步, 不传输数据
  - 无排队性, 多次向任务设置同一事件(未响应) = 设置一次
  - 允许多任务对同一事件读写
  - 支持事件等待超时机制
  