#include "include.h"

#define portINITIAL_XPSR   (0x01000000)
#define portSTART_ADDRESS_MASK      ((StackType_t)0xfffffffeUL)

static void prvTaskExitError(void)
{
    while (1)
    {
        /* code */
    }
    
}

StackType_t * pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    // 异常发生时,自动加载CPU寄存器内容
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ((StackType_t)pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)prvTaskExitError;
    pxTopOfStack -= 5;
    *pxTopOfStack = (StackType_t)pvParameters;
    // 异常发生时,手动加载到CPU寄存器
    pxTopOfStack -= 8;
    // 返回栈顶指针,指向空闲栈
    return pxTopOfStack;
}