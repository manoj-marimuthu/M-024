#ifndef VARIABLE_REGISTRY_H
#define VARIABLE_REGISTRY_H

#include <stdbool.h>
#include <value.h>
#include <memoryHandler.h>

typedef struct Variable{
    char* varName;
    DataType type;
    Value* data;
    bool isConstant;
    char* funcName;
    struct Variable* next;
} Variable;

Variable* arr[100];
Variable* local_var[100];
bool inFunction;
void clear_local(char* funcName);
Variable* createVariable(char* varName,DataType type,MemNode* obj);
unsigned int hash(char* varName);
void setVariable(Variable* v);
void initVariableRegistry();
Variable* getVariable(char* varName);

#endif