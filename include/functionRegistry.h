#ifndef FUNC_REGISTRY_H
#define FUNC_REGISTRY_H

#include <parser.h>

typedef struct Param{
    char* paramName;
    struct Param* next;
} Param;

typedef struct Function{
    char* functionName;
    Param* params;
    astNode* thenBlock;
    int param_length;
    struct Function* next;
    int i_read_bit;
    int i_write_bit;
    int i_kill_bit;
    int e_read_bit;
    int e_write_bit;
    int e_kill_bit;
    char* from;
} Function;

extern Function* functionArr[100];
void initFunctionRegister();
Param* createParam(char* paramName);
Function* createFunction();
void setFunction(Function* f);
Function* getFunction(char* functionName);
int removeFunction(char* functionName);
#endif
