#ifndef EVALUATE_H
#define EVALUATE_H

#include <parser.h>
#include <value.h>
#include <stdbool.h>
#include <memoryHandler.h>
#include <variableRegistry.h>
astNode* ast_root;
astNode* ast_tail;
Value evaluate(astNode* node);
Value makeNone();
Value makeValue();
bool isTruthy(Value val);
void Execute();
void interpret(char* fileName);
#endif