#ifndef EVALUATE_H
#define EVALUATE_H

#include <parser.h>
#include <value.h>
#include <stdbool.h>
#include <memoryHandler.h>
#include <variableRegistry.h>
#include <listobj.h>
Value evaluate(astNode* node);
Value makeNone();
Value makeNumber(double num);
Value makeString(char* str);
Value makeBoolean(bool boolean);
void printValue(Value v);
char* cloneString(char* str);
bool isTruthy(Value val);
double toNumber(Value val);
ListNode* valueToListNode(Value v);
Value listNodeToValue(ListNode* listNode);
void Execute();
void interpret(char* fileName);
#endif