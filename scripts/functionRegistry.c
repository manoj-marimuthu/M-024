#include <functionRegistry.h>
#include <memoryHandler.h>
#include <error.h>
#include <variableRegistry.h>
#include <string.h>
// function registry
Function* functionArr[100];
void initFunctionRegister(){
    for(int i =0;i < 100;i++){
        functionArr[i] = NULL;
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
    f->i_read_bit = 1;
    f->i_write_bit = 1;
    f->i_write_bit = 1;
    return f;
}

void setFunction(Function* f){
    unsigned int index = hash(f->functionName);
    if(!functionArr[index]){
        functionArr[index] = f;
        return;
    }else{
        Function** cur = &functionArr[index];
	Function* prev = NULL;
        while(*cur){
            if(strcmp((*cur)->functionName,f->functionName) == 0){
                if(!(*cur)->i_write_bit){
		   char errMsg[256];
		   snprintf(errMsg,sizeof(errMsg),"Permission to write is denied for '%s'",f->functionName);
		   error(errMsg,-1,RUN_TIME_ERROR);
		}else{
		   if(prev != NULL){
			prev->next = f;
		   }
		   Function* next = (*cur)->next;
		   f->next = next;
		   *cur = f;
		}
            }
	    prev = *cur;
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
	    if(!cur->i_read_bit){
		char errMsg[256];
		snprintf(errMsg,sizeof(errMsg),"Permission to read '%s' is denied",functionName);
		error(errMsg,-1,RUN_TIME_ERROR);
	    }
            return cur;
        }
        cur = cur->next;
    }
   char err[256];
    snprintf(err,sizeof(err),"Undefined Function '%s'",functionName);
    error(err,0,FUNCTION_ERROR);
    return NULL;
}


int removeFunction(char* functionName){
    size_t index = hash(functionName);
    Function* cur = functionArr[index];
    Function* prev = NULL;
    bool found = 0;
    while(cur != NULL){
        if(strcmp(functionName,cur->functionName) == 0){
	    if(!cur->i_kill_bit){
		char errMsg[256];
		snprintf(errMsg,sizeof(errMsg),"Permission to kill '%s' is denied",functionName);
		error(errMsg,-1,RUN_TIME_ERROR);
	    }
            if(prev == NULL){
                functionArr[index] = cur->next;
	    }else{
                prev->next = cur->next;
            }
            found = true;
        }
        if(found) break;
        prev = cur;
        cur = cur->next;
    }
    if(found) return 1;
    else return 0;
}
