#include "projdefs.h"
#include "task.h"

#if(configSUPPORT_STATIC_ALLOCATION ==1)    // 静态内存

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
