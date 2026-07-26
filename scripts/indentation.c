#include <memoryHandler.h>
#include <indentation.h>
#include <error.h>
Stack* createStack(){
    MemNode* node = createMemNode(sizeof(Stack));
    Stack* ptr = node->ptr;
    ptr->top = NULL;
    ptr->tail = NULL;
    return ptr;
}

void pushStack(Stack* stk,int num){
    if(stk == NULL){
        error("Undefined Error In Indentation Stack Declaration",0,INDENTATION_ERROR);
    }else{
       MemNode* stackNodeObj = createMemNode(sizeof(StackNode));
       StackNode* stackNode = stackNodeObj->ptr;
       stackNode->next = NULL;
       stackNode->value = num;
       if(stk->top == NULL){
            stk->top = stackNode;
            stk->tail = stackNode;
       }else{
            stk->tail->next = stackNode;
            stk->tail = stackNode;
       }
    }
}

int popStack(Stack* stk){
    if(stk == NULL || stk->top == NULL){
        error("Indentation Stack Underflow",0,INDENTATION_ERROR);
    }
    
    int value = stk->tail->value;
    if(stk->top == stk->tail){
        stk->top = NULL;
        stk->tail = NULL;
        return value;
    }
    StackNode* current = stk->top;
    while(current->next != stk->tail){
        current = current->next;
    }
    current->next =NULL;
    stk->tail = current;
    return value;
}
int peekStack(Stack* stk){
    if(stk == NULL || stk->tail == NULL){
        error("Indentation Stack Peek Operation Failed",0,INDENTATION_ERROR);
    }
    return stk->tail->value;
}

void traverseStack(Stack* stk){
    StackNode* current=  stk->top;
    while(current != NULL){
        printf("%d \n",current->value);
        current = current->next;
    }
}