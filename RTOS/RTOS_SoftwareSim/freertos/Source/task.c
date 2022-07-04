#include "include.h"

#if(configSUPPORT_STATIC_ALLOCATION ==1)    // 静态内存

/**
 * @brief 
 * 
 * @param pxTaskCode 任务入口
 * @param pcName 任务名
 * @param ulStackDepth 任务栈大小
 * @param pvParameters 任务形参
 * @param pxCreatedTask 任务句柄
 * @param pxNewTCB 任务控制块地址
 */
static void prvInitialiseNewTask(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth, void * const pvParameters, TaskHandle_t * const pxCreatedTask, TCB_t *pxNewTCB)
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;

    // 获取栈顶地址
    pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - (uint32_t) 1 );
    // 向下做8字节对齐
    pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack)&(~((uint32_t)0x0007)));
    // 任务名存储在TCB
    for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)
    {
        pxNewTCB->pcTaskName[x] = pcName[x];
        if(pcName[x]==0x00)
        {
            break;
        }
    }
    // 人物名字长度不超过configMAX_TASK_NAME_LEN
    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1]='\0';
    // 初始化TCB的xStateListItem节点
    vListInitialiseItem( &(pxNewTCB->xStateListItem));
    // 设置xStateListItem节点拥有者
    listSET_LIST_ITEM_OWNER( &(pxNewTCB->xStateListItem), pxNewTCB);
    // 初始化任务栈
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);
    // 任务句柄指向控制块
    if((void*)pxCreatedTask != NULL)
    {
        *pxCreatedTask = (TaskHandle_t)pxNewTCB;
    }
}



/**
 * @brief 
 * 
 * @param pxTaskCode 任务入口
 * @param pcName 任务名
 * @param ulStackDepth 任务栈大小
 * @param pvParameters 任务形参
 * @param puxStackBuffer 
 * @param pxTaskBuffer 
 * @return TaskHandle_t 
 */
TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth, void * const pvParameters, StackType_t * const puxStackBuffer, TCB_t * const pxTaskBuffer)
{
    TCB_t * pxNewTCB;
    TaskHandle_t xReturn;
    if((pxTaskBuffer != NULL)&&(puxStackBuffer != NULL))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;
        // 创建新任务
        prvInitialiseNewTask(pxTaskCode,pcName,ulStackDepth,pvParameters,&xReturn,pxNewTCB);
    }
    else
    {
        xReturn = NULL;
    }
    // 返回任务句柄,若成功则xReturn指向任务控制块
    return xReturn;
}

#endif
