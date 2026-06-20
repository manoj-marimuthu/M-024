#ifndef EVALUATE_H
#define EVALUATE_H

#include <parser.h>
#include <value.h>
#include <stdbool.h>
#include <memoryHandler.h>
#include <variableRegistry.h>

static Value evaluate(astNode* node);
static Value makeNone();
static Value makeNumber(double num);
static Value makeString(char* str);
static Value makeBoolean(bool boolean);
static void printValue(Value v);
static char* cloneString(char* str);
static bool isTruthy(Value val);
static double toNumber(Value val);
void Execute();
void interpret(char* fileName);
#endif