#include <callStack.h>
#include <memoryHandler.h>
#include <error.h>
CallStackNode* call_stack = NULL;

CallStackNode* createCallStackNode(){
    MemNode* callStackNodeObj = createMemNode(sizeof(CallStackNode));
    CallStackNode* node = callStackNodeObj->ptr;
    node->function = NULL;
    for(int i =0;i <  100;i++){
        node->locals[i] = NULL;
    }
    node->prev = NULL;
    return node;
}

void pushCallStackNode(CallStackNode* node){
    if(call_stack == NULL){
        call_stack = node;
    }else{
        node->prev = call_stack;
        call_stack = node;
    }
}

void popCallStackNode(){
    if(call_stack == NULL){
        error("Popping Call Stack When It is Empty !",-1,FUNCTION_ERROR);
        return;
    }
    call_stack = call_stack->prev;
}