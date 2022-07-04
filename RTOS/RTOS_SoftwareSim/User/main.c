#include "list.h"
#include "FreeRTOS.h"
#include "task.h"

struct xLIST List_Test;
struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3;

TCB_t Task1TCB;
TCB_t Task2TCB;

void delay(uint32_t count)
{
	for(;count!=0;count--);
}

int flag1,flag2;

void Task1_Entry(void *p_arg)
{
	for (;;)
	{
		flag1=1;
		delay(100);
		flag1=0;
		delay(100);
	}
}

void Task2_Entry(void *p_arg)
{
	for (;;)
	{
		flag2=1;
		delay(100);
		flag2=0;
		delay(100);
	}
}

int main(void)
{
	// 链表根节点初始化
	vListInitialise(&List_Test);
	// 节点1初始化
	vListInitialiseItem(&List_Item1);
	List_Item1.xItemValue = 1;
	// 节点2初始化
	vListInitialiseItem(&List_Item2);
	List_Item2.xItemValue = 2;
	// 节点3初始化
	vListInitialiseItem(&List_Item3);
	List_Item3.xItemValue = 3;
	// 节点插入链表,按升序排列
	vListInsert(&List_Test, &List_Item2);
	vListInsert(&List_Test, &List_Item3);
	vListInsert(&List_Test, &List_Item1);

	while(1)
	{
		
	}
}