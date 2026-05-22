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
} Function;
Function* functionArr[100];
void initFunctionRegister();
Param* createParam(char* paramName);
Function* createFunction();
void setFunction(Function* f);
Function* getFunction(char* functionName);

#endif