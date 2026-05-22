#include <variableRegistry.h>
#include <memoryHandler.h>
#include <error.h>
#include <string.h>
#include <value.h>
#include <callStack.h>

Variable* createVariable(char* varName,DataType type,MemNode* obj){
    MemNode* node = createMemNode(sizeof(Variable));
    Variable* v = node->ptr;
    MemNode* strObj = createMemNode(strlen(varName)+1);
    v->varName = strObj->ptr;
    strcpy(v->varName,varName);
    v->data =  obj->ptr;
    v->isConstant = false;
    v->type = type;
    v->next = NULL;
    return v;
}

unsigned int hash(char* varName){
    int temp = 0;
    int n = strlen(varName);
    for(int i =0;i < n;i++){
        temp += (int) varName[i];
    }
    unsigned int key = (temp * 33) % 100;
    return key;
}

void setVariable(Variable* v){
    size_t index = hash(v->varName);
    Variable* current = call_stack->locals[index];
    Variable* prev = NULL;
    while(current != NULL){
        if(strcmp(current->varName,v->varName) == 0){
            if(current->isConstant){
                char err[256];
                snprintf(err,sizeof(err),"'%s' cannot be Modified (const)",current->varName);
                error(err,0,RUN_TIME_ERROR);
                return;
            }
            current->data = v->data;
            current->type = v->type;
            return;
        }
        prev = current;
        current = current->next;
    }
    if(prev == NULL){
        call_stack->locals[index] = v;
    }else{
        prev->next = v;
    }
}

Variable* getVariable(char* varName){
    size_t index = hash(varName);
    CallStackNode* curStack = call_stack;
    while(curStack){
        Variable* current = curStack->locals[index];
        while(current != NULL){
            if(strcmp(current->varName,varName) == 0){
                return current;
            }
            current = current->next;
        }
        curStack = curStack->prev;
    }
    char err[256];
    snprintf(err,sizeof(err),"'%s'  Accesing an Undefined Variable, Check Variable Names and Previous Declaration",varName);
    error(err,0,RUN_TIME_ERROR);
    return NULL;
}
