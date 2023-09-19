#include <stdio.h>
#include <malloc.h>

typeof struct Node
{
    int data; // 数据域
    struct Node * pNext; // 指针域
}NODE, *PNODE; // NODE等价于struct Node PNODE 等价于struct Node *

int main(void)
{
    PNODE pHead = NULL; // 等价于 struct Node * pHead = NULL

    pHead = create_list();

    return 0;
}

PNODE create_list(void)
{
    int len; // 用来存放有效节点个数
    int i;
    int val; // 用来临时存放用户输入的结点的值

    // 分配了一个不存放有效数据的头结点
    PNODE pHead = (PNODE)malloc(sizeof(NODE));
    if(NULL == pHead)
    {
        printf("分配失败，程序终止!\n");
        exit(-1);
    }
    PNODE pTail = pHead;
    pTail->pNext = NULL;
}