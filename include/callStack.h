#ifndef CALL_STACK_H
#define CALL_STACK_H

#include <functionRegistry.h>
#include <variableRegistry.h>

typedef struct CallStackNode{
    Function* function;
    Variable* locals[100];
    struct CallStackNode* prev;
} CallStackNode;

CallStackNode* call_stack;
CallStackNode* createCallStackNode();
void pushCallStackNode(CallStackNode* node);
void popCallStackNode();
#endif