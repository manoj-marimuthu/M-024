#ifndef LIST_OBJ_H
#define LIST_OBJ_H

#include <value.h>

typedef struct ListNode{
    struct ListNode* next;
    Value* value;
} ListNode;

typedef struct List{
    ListNode* top;
    ListNode* tail;
    int length;
} List;

ListNode* createListNode();
List* createList();
void pushList(List* list,ListNode* node);
ListNode* popList(List* list);
ListNode* getListIndex(List* list,int index);

#endif