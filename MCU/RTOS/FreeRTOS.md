# 1.FreeRTOS
![源码下载](https://www.freertos.org/a00104.html)
ref: 野火 《FreeRTOS 内核实现与应用开发实战—基于STM32》
可在野火大学堂下载 或 [下载链接](https://gitee.com/Wind_to_valley/notes/blob/master/MCU/RTOS/%E3%80%8AFreeRTOS%20%E5%86%85%E6%A0%B8%E5%AE%9E%E7%8E%B0%E4%B8%8E%E5%BA%94%E7%94%A8%E5%BC%80%E5%8F%91%E5%AE%9E%E6%88%98%E2%80%94%E5%9F%BA%E4%BA%8ESTM32%E3%80%8B.pdf)
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
    - ![在这里插入图片描述](https://img-blog.csdnimg.cn/cb6a70c208d945258becee67a70a122f.png)
- 特点
  - 事件相互独立, 只与任务关联, 32位时间集合`EventBits_t`24位表示事件: 0:未发生 1:已发生 共24种事件类型
  - 仅用于同步, 不传输数据
  - 无排队性, 多次向任务设置同一事件(未响应) = 设置一次
  - 允许多任务对同一事件读写
  - 支持事件等待超时机制
- 函数
  - `xEventGroupCreate()`创建事件
  - `vEventGroupDelete()`删除事件
  - `xEventGroupSetBits()`置位事件组, 阻塞于该位的任务被解锁
  - `xEventGroupSetBitsFromISR()`中断置位事件组, 阻塞于该位的任务被解锁
  - `xEventGroupWaitBits()`阻塞等待事件, 获取事件是否已经发生
  - `xEventGroupClearBits()`清除事件组指定的位
  - `xEventGroupClearBitsFromISR()`中断清除事件组指定的位

# 软件定时器
- 经过设定的时钟计数值后触发用户定义的回调函数, 适于对时间精度要求不高的任务
  - 节拍周期的整数倍
  - 单次模式: 创建启动定时器后, 定时时间到, 只执行一次进入休眠, 
  - 周期模式: 列表中删除, 下一次定时到时升序插入
  - 按设置定时时间循环执行回调函数直至删除
  - ![在这里插入图片描述](https://img-blog.csdnimg.cn/f9bc69d2c80249c886d29a5b95ce8056.png)
- 函数
  - `xTimerCreate()`创建软件定时器
  - `xTimerStart()`启动软件定时器
  - `xTimerStartFromISR()`中断启动软件定时器
  - `xTimerStop()`停止一个已启动的软件定时器
  - `xTimerStopFromISR()`中断停止一个已启动的软件定时器
  - `xTimerDelete()`删除软件定时器函数

# 任务通知
- 每个任务有一个32位的通知值, 可替代二值信号量, 计数信号量, 事件组, 长度1队列
  - 发送通知给任务, 若有通知未读, 不覆盖通知值
  - 发送通知给任务, 覆盖通知值
  - 发送通知给任务, 设置通知值一个/多个位, 作为事件组
  - 发送通知给任务, 递增通知值, 作为计数信号量
  - 只能有一个任务接收通知消息, 须指定接收通知的任务
  - 只有等待通知的任务可阻塞, 发通知的任务不阻塞
- 函数
  - `xTaskGenericNotify()`发送任务通知函数
  - `vTaskNotifyGiveFromISR()`中断发送任务通知函数
  - `xTaskNotify()`直接向另一个任务发送一个事件
  - `xTaskNotifyFromISR()`中断直接向另一个任务发送一个事件
  - `ulTaskNotifyTake()`获取任务通知, 配合Give()
  - `xTaskNotifyWait()`等待通知

# 内存管理
- 用于系统中内存资源, 初始化, 分配, 释放
  - `heap_1.c`
  - `heap_2.c`
  - `heap_4.c`内存堆为一个大数组`ucHeap[configTOTAL_HEAP_SIZE]`
  - `heap_3.c`封装了c标准库的`malloc()`, `free()`, 可安全地在嵌入式系统执行
  - `heap_5.c`允许用户使用多个非连续内存堆空间, 每个堆起始地址,大小由用户定义
- `heap_1.c`
  - 只能申请内存不能释放, 申请时间为常量, 不会产生碎片导致崩溃, 不能回收, 内存利用率低
  - 用于从不删除任务, 队列, 信号量, 互斥量等的程序
  - 函数执行时间确定, 不产生内存碎片
  - `pvPortMalloc()`申请一块用户指定大小的内存空间, 系统管理的空间满足需求则成功
- `heap_2.c`
  - 最佳匹配算法, 优先分配申请量接近的内存块
  - 支持释放申请内存, 不能把相邻小内存块合并为大内存块
  - 适于反复删除任务, 队列, 信号量等内核对象, 不担心内存碎片的程序
  - 不确定性, 效率>c库`malloc()`
  - 不能用于内存分配, 释放随机大小的程序
  - `pvPortMalloc()`最佳匹配算法管理内存, 先从内存块空闲链表头开始遍历, 查找符合用户申请大小的内存块, 申请成功后判断当前内存是否剩余, 完善该内存块信息, 插入内存块空闲链表
    - ![在这里插入图片描述](https://img-blog.csdnimg.cn/1ba18efc0bbf4b9db922c16f70886f1b.png)
  - `vPortFree()`传入要释放的内存地址, 自动索引到对应链表节点, 取出信息, 插入空闲内存块链表
    - ![在这里插入图片描述](https://img-blog.csdnimg.cn/95184c301ecf4c1cb9713ceaa283aca4.png)
- `heap_3.c`
  - 封装标准c库的`malloc()`功能和`free()`函数, 有保护功能
  - 需要链接器设置一个堆
  - 不确定性
  - 增大RTOS内核代码大小
- `heap_4.c`
  - 最佳匹配算法实现动态内存分配, 合并算法可将相邻空闲内存块合并为更大的块, 减少内存碎片
  - 适于重复删除任务, 队列, 信号量, 互斥量等的应用
  - 用于分配, 释放随即字节内存的应用, 不会有严重内存碎片
  - 不确定性, 效率>标准c库`malloc()`
- `heap_5.c`
  - 最佳匹配算法, 合并算法, 允许内存跨多个非连续内存区, 在不连续内存堆实现分配
  - `vPortDefineHeapRegions()`实现系统管理的内存初始化

# 中断管理
- 异常: 打断处理器正常执行, 迫使处理器进入一个由有特权的特殊指令执行的事件
  - 同步异常: 内部事件(处理器指令运行产生的事件引起), 触发后须立刻处理, 不能执行原有程序指令步骤
  - 异步异常: 外部硬件装置产生的事件引起的异常源, 触发后可以延缓/忽略
    - 中断: 可以打断任何优先级的任务, 临界段中会挂起中断
      - 相关硬件
        - 外设: 外设需要请求CPU, 产生一个中断信号, 连接到中断控制器
        - 中断控制器: CPU外设之一, 接收其他外设中断信号输入, 发出中断信号给CPU, 控制中断源优先级,触发方式,开关源
        - CPU: 响应中断源请求, 当前任务->中断处理程序
      - 概念
        - 中断号: 每个中断请求信号有特定标志, 使计算机可判断哪个设备提出的中断请求
        - 中断请求: 紧急事件向CPU提出申请要求CPU暂停当前执行任务, 处理紧急事件
        - 中断优先级: 使系统及时响应, 处理所有中断, 由中断时间的重要性&紧迫程度, 分若干级别
        - 中断处理程序: 产生中断请求后, CPU暂停当前任务响应中断申请, 执行中断处理程序
        - 中断触发: 中断源发出, 送给CPU控制性能好, 触发器置1表明产生了中断
        - 中断触发类型: 外部中断申请通过一个物理信号发到NVIC, 电平触发/边沿触发
        - 中断向量: 中断服务程序入口地址
        - 中断向量表: 存储中断向量, 对应中断号
        - 临界段: 该部分代码执行时不允许任何中断
      - 任务间发生 ![在这里插入图片描述](https://img-blog.csdnimg.cn/da1e70bb3c554ab196051a5453f1749b.png)
      - 嵌套发生 ![在这里插入图片描述](https://img-blog.csdnimg.cn/8e1c18da7f784b158cc387c06839c256.png)
      - 中断延迟 = 识别中断事件 + [等待中断打开时间] + [关闭中断时间]

# CPU使用率
- 外部变量统计时间, 消耗一个高精度定时器, 系统时钟节拍x10~20
- `vTaskGetRunTimeStats()`获取任务相关信息
- `vTaskList()`获取CPU使用率信息
