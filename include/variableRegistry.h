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
    struct Variable* next;
} Variable;

Variable* createVariable(char* varName,DataType type,MemNode* obj);
unsigned int hash(char* varName);
void setVariable(Variable* v);
Variable* getVariable(char* varName);
void removeVariable(char* varName);
#endif