#ifndef INDENT_H
#define INDENT_H


typedef struct  StackNode{
    int value;
    struct StackNode* next;
} StackNode;

typedef struct Stack{
    StackNode* top;
    StackNode* tail;
} Stack;

Stack* createStack();
void pushStack(Stack* stk,int num);
int popStack(Stack* stk);
int peekStack(Stack* stk);
void traverseStack(Stack* stk);
#endif
