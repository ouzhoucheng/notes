struct xLIST_ITEM
{
    TickType_t xItemValue; // 辅助值:帮节点做顺序排列
    struct xLIST_ITEM * pxNext; // 指向链表下一个节点
    struct xLIST_ITEM * pxPrevious; // 指向链表前一个节点
    void * pvOwner; // 指向拥有该节点的内核对象(TCB)
    void * pvContainer; // 指向该节点所在链表
};
typedef struct xLIST_ITEM ListItem_t; // 节点数据类型重定义

徐老师您好，请问今年8月 19级测控的小学期，您在这边有题目吗

我想找您当指导老师

