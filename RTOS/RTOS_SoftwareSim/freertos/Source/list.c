#include "list.h"
/**
 * @brief 链表节点初始化
 * 
 * @param pxItem 
 */
void vListInitialiseItem( ListItem_t * const pxItem ) 
{ 
    /* 初始化该节点所在的链表为空，表示节点还没有插入任何链表 */
    pxItem->pvContainer = NULL;
} 

/**
 * @brief 链表根节点初始化
 * 
 * @param pxList 
 */
void vListInitialise(List_t * const pxList)
{
    // 链表索引指针指向最后一个节点
    pxList->pxIndex = (ListItem_t *) & (pxList->xListEnd);
    // 链表最后一个节点辅助排序值设为最大,确保该节点为链表最后节点
    pxList->xListEnd.xItemValue = portMAX_DELAY;
    // 将最后一个节点的pxNext和pxPrevious指针指向节点自身,表示链表为空
    pxList->xListEnd.pxNext = (ListItem_t *) & (pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *) & (pxList->xListEnd);
    // 初始化链表节点计数器值=0,表示链表为空
    pxList->uxNumberOfItems = (UBaseType_t) 0U;
}

/**
 * @brief 将节点插入到链表尾部
 * 
 * @param pxList 
 * @param pxNewListItem 
 */
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem)
{
    ListItem_t * const pxIndex = pxList->pxIndex;
    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;

    //记住该节点所在链表
    pxNewListItem->pvContainer = (void * ) pxList;
    //链表节点计数器++
    (pxList->uxNumberOfItems)++;
}

/**
 * @brief 将节点按升序排列插入链表
 * 
 * @param pxList 
 * @param pxNewListItem 
 */
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem)
{
    ListItem_t *pxIterator;
    // 获取节点排序辅助值
    const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;
    // 寻找节点插入位置
    if(xValueOfInsertion == portMAX_DELAY)
    {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    else
    {
        for(pxIterator = (ListItem_t *) &(pxList->xListEnd);pxIterator->pxNext->xItemValue <= xValueOfInsertion;pxIterator = pxIterator->pxNext)
        {
            //找到节点要插入的位置
        }
    }
    // 由升序排列,插入节点
    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;
    // 记住该节点所在链表
    pxNewListItem->pvContainer = (void*)pxList;
    // 链表节点计数器++
    (pxList->uxNumberOfItems)++;
}

/**
 * @brief 将节点从列表删除
 * 
 */
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove)
{
    // 获取节点所在链表
    List_t * const pxList = (List_t *) pxItemToRemove->pvContainer;
    // 将指定节点从链表删除
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
    // 调整链表节点索引指针
    if(pxList->pxIndex == pxItemToRemove)
    {
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }
    // 初始化该节点所在链表为空,表示节点未插入链表
    pxItemToRemove->pvContainer = NULL;
    // 链表节点计数器--
    (pxList->uxNumberOfItems)--;
    // 返回链表中剩余节点个数
    return pxList->uxNumberOfItems;
}
