#include "portmacro.h"
#ifndef LIST_H
#define LIST_H
/**
 * @brief 链表节点数据结构定义
 * 
 */
struct xLIST_ITEM 
{
    TickType_t xItemValue; // 辅助值:帮节点做顺序排列
    struct xLIST_ITEM * pxNext; // 指向链表下一个节点
    struct xLIST_ITEM * pxPrevious; // 指向链表前一个节点
    void * pvOwner; // 指向拥有该节点的内核对象(TCB)
    void * pvContainer; // 指向该节点所在链表
};
typedef struct xLIST_ITEM ListItem_t; /* 节点数据类型重定义 */ 

/**
 * @brief 链表首个节点, 精简节点
 * 
 */
struct xMINI_LIST_ITEM 
{
    TickType_t xItemValue; // 辅助值:帮节点做顺序排列
    struct xLIST_ITEM * pxNext; // 指向链表下一个节点
    struct xLIST_ITEM * pxPrevious; // 指向链表前一个节点
};
typedef struct xMINI_LIST_ITEM MiniListItem_t; /* 节点数据类型重定义 */ 

/**
 * @brief 链表根节点数据结构定义
 * 
 */
typedef struct xLIST
{
    UBaseType_t uxNumberOfItems; // 链表节点计数器
    ListItem_t * pxIndex; // 链表节点索引指针
    MiniListItem_t xListEnd; // 链表最后一个节点
}List_t;

/* 初始化节点的拥有者 */ 
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner ) ( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )

/* 获取节点拥有者 */ 
#define listGET_LIST_ITEM_OWNER( pxListItem ) ( ( pxListItem )->pvOwner )

/* 初始化节点排序辅助值 */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue ) ( ( pxListItem )->xItemValue = ( xValue ) )

/* 获取节点排序辅助值 */ 
#define listGET_LIST_ITEM_VALUE( pxListItem ) ( ( pxListItem )->xItemValue )

/* 获取链表根节点的节点计数器的值 */ 
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList ) ( ( ( pxList )->xListEnd ).pxNext->xItemValue )

/* 获取链表的入口节点 */ 
#define listGET_HEAD_ENTRY( pxList ) ( ( ( pxList )->xListEnd ).pxNext )

/* 获取节点的下一个节点 */ 
#define listGET_NEXT( pxListItem ) ( ( pxListItem )->pxNext )

/* 获取链表的最后一个节点 */ 
#define listGET_END_MARKER( pxList ) ( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )

#endif
