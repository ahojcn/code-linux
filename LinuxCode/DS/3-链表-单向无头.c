#include <stdio.h>
#include <stdlib.h>

typedef int SLDataType;
typedef struct SLNode {
    SLDataType val;
    struct SLNode* next;
} SLNode;

typedef struct SList {
    SLNode* first;
} SList;

// 初始化
void
SListInit(SList* list) {
    assert(list);
    list->first = NULL;
}
// 销毁

// 增
// 头插
void
SListPushFront(SList* list, SLDataType val) {
    assert(list != NULL);
    SLNode* node = (SLNode*)malloc(sizeof(SLNode));
    assert(node);
    node->val = val;
    node->next = list->first;
    list->first = node;
}

// 删
// 头删
void
SListPopFront(SList* list) {
    assert(list != NULL);
    assert(list->first != NULL);
    SLNode* t = list->first;
    list->first = list->first->next;
    free(t);
}

// 改

// 查

// 测试
void
TestSList1() {
    SList list;
    SListInit(&list);
    assert(list.first == NULL);

    SListPushFront(&list, 1);
    SListPushFront(&list, 2);
    SListPushFront(&list, 3);
}

int main() {
    return 0;
}
