#include <variableRegistry.h>
#include <memoryHandler.h>
#include <error.h>
#include <string.h>
#include <value.h>
#include <callStack.h>
#include <listobj.h>

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
    v->indexes = NULL;
    v->isIndexed = false;
    v->read_bit = 7;
    v->write_bit = 7;
    v->kill_bit = 7;
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
    if(v->isIndexed){
        Variable* existing = getVariable(v->varName);
        if(existing->isConstant || !existing->write_bit){
                char err[256];
                snprintf(err,sizeof(err),"'%s' cannot be Modified (const)",existing->varName);
                error(err,0,RUN_TIME_ERROR);
                return;
            }
        if(existing->type != D_LIST) error("Cannot assign values at indexes of non-list object",-1,RUN_TIME_ERROR);
        List* listToSet = existing->data->data.listData;
        Index* idx = v->indexes;
        ListNode* toSet = getListIndex(listToSet,idx->index);
        while(idx != NULL){
            toSet = getListIndex(listToSet,idx->index);
            if(idx->next){
                if(toSet->value->type != D_LIST){
                    error("Cannot assign values at indexes of non-list object",-1,RUN_TIME_ERROR);
                }
                listToSet = toSet->value->data.listData;
            }
            idx = idx->next;
        }
        toSet->value = v->data;
        return;
    }
    while(current != NULL){
        if(strcmp(current->varName,v->varName) == 0){
            if(current->isConstant || !current->write_bit){
                char err[256];
                snprintf(err,sizeof(err),"'%s' cannot be Modified (const)",current->varName);
                error(err,0,RUN_TIME_ERROR);
                return;
            }
            *(current->data) = *(v->data);
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

int removeVariable(char* varName){
    size_t index = hash(varName);
    CallStackNode* curStack = call_stack;
    int found = 0;
    while(curStack){
        Variable* current = curStack->locals[index];
        found = 0;
        if(current && strcmp(current->varName,varName) == 0){
	    if(!current->kill_bit){
		char errMsg[256];
		snprintf(errMsg,sizeof(errMsg),"Kill permission denied for variable '%s'",varName);
		error(errMsg,-1,RUN_TIME_ERROR);
	    }
            curStack->locals[index] = current->next;
            found = 1;
        }
        if(found) break;
        Variable* prev = NULL;
        while(current != NULL){
            if(strcmp(current->varName,varName) == 0){
                if(prev){
                    prev->next = current->next;
                }else{
                    curStack->locals[index] = current->next;
                }
                found = 1;
                break;
            }
            current = current->next;
            prev = current;
        }
        if(found) break;
        curStack = curStack->prev;
    }
    if(found) return 1;
    else return 0;
}
