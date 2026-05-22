#include <functionRegistry.h>
#include <memoryHandler.h>
#include <error.h>
#include <variableRegistry.h>
#include <string.h>
// function registry
Function* functionArr[100];
void initFunctionRegister(){
    for(int i =0;i < 100;i++){
        arr[i] = NULL;
    }
}
Param* createParam(char* paramName){
    MemNode* node = createMemNode(sizeof(Param));
    Param* p = node->ptr;
    p->next = NULL;
    p->paramName = paramName;
    return p;
}
Function* createFunction(){
    MemNode* node = createMemNode(sizeof(Function));
    Function* f = node->ptr;
    f->next = NULL;
    f->params = NULL;
    f->thenBlock = NULL;
    f->functionName = NULL;
    f->param_length = 0;
    return f;
}

void setFunction(Function* f){
    unsigned int index = hash(f->functionName);
    if(!functionArr[index]){
        functionArr[index] = f;
        return;
    }else{
        Function** cur = &functionArr[index];
        while(*cur){
            if(strcmp((*cur)->functionName,f->functionName) == 0){
                char err[256];
                snprintf(err,sizeof(err),"Function '%s' Already Exists",f->functionName);
                error(err,0,FUNCTION_ERROR);
            }
            cur = &(*cur)->next;
        }
        *cur = f;
        return;
    }
}

Function* getFunction(char* functionName){
    unsigned int index = hash(functionName);
    Function* cur = functionArr[index];
    while(cur != NULL){
        if(strcmp(functionName,cur->functionName) == 0){
            return cur;
        }
    }
   char err[256];
    snprintf(err,sizeof(err),"Undefined Function '%s'",functionName);
    error(err,0,FUNCTION_ERROR);
    return NULL;
}