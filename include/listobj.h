#ifndef LIST_OBJ_H
#define LIST_OBJ_H

#include <value.h>

typedef struct ListNode{
    struct ListNode* prev;
    struct ListNode* next;
    Value value;
} ListNode;

typedef struct List{
    ListNode* top;
    int length;
} List;
#endif