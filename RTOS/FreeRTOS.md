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
