#include <listobj.h>
#include <memoryHandler.h>
#include <error.h>

ListNode* createListNode(){
    MemNode* listNodeObj = createMemNode(sizeof(ListNode));
    ListNode* listNode = listNodeObj->ptr;
    listNode->next = NULL;
    listNode->value = NULL;
    return listNode;
}

List* createList(){
    MemNode* listObj = createMemNode(sizeof(List));
    List* list = listObj->ptr;
    list->length = 0;
    list->top = NULL;
    list->tail = NULL;
    return list;
}

void pushList(List* list,ListNode* node){
    if(list == NULL){
        error("listobj is pointing to null,hence push is impossible",-1,COMPILE_TIME_ERROR);
    }
    if(list->top == NULL){
        list->top = node;
        list->tail = node;
        list->length++;
    }else{
        list->tail->next = node;
        list->tail = node;
        list->length++;
    }
}

ListNode* popList(List* list){
    if(list == NULL){
        error("listobj is pointing to null,hence pop is impossible",-1,COMPILE_TIME_ERROR);
    }
    if(list->top){
        ListNode* toReturn = list->tail;
        ListNode* curListNode = list->top;
        while(curListNode && curListNode->next != list->tail) curListNode =  curListNode->next;
        curListNode->next = NULL;
        list->tail = curListNode;
        list->length--;
        return toReturn;
    }else{
        error("Cannot pop an empty list object",-1,RUN_TIME_ERROR);
    }
    return NULL;
}

ListNode* getListIndex(List* list,int index){
    if(list == NULL){
        error("Cannot index a listobj which is null",-1,COMPILE_TIME_ERROR);
    }else{
        if(index >= list->length){
            error("Cannot use an index greater than the size of the listobj \n(indexes begin from 0 and go until n-1 where n is the size of the listobj)",-1,RUN_TIME_ERROR);
        }
        int c = 0;
        ListNode* listNode = list->top;
        while(c < index){
            listNode = listNode->next;
            c++; // damn, C++ in C
        }
        return listNode;
    }
    return NULL;
}
