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
```C++
struct node
{
  struct node *next;  // 指向链表的下一个节点
  char data1; // 
  unsigned char array[];  // 
  unsigned long *prt; // 
  struct userstruct data2;  // 
}
```
通常在节点中只包含一个用于指向下一个节点的指针,通过链表存储的数据内嵌一个节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/2139e729c01946b7ac82e6ab4fbc9a85.png)
```C++
struct node // 节点定义
{
  struct node *next;  // 指向链表的下一个节点
}
struct userstruct
{
  struct node *next;  // 结构体中内嵌一个结点指针,通过该节点将数据挂接到链表
  /* 数据 */
}
```