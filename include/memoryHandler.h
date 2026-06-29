#ifndef GC_H
#define GC_H

#include <stdio.h>
#include <stdbool.h>
#include <value.h>

typedef struct MemNode
{
    void* ptr;
    struct MemNode* next;
    struct MemNode* prev;
} MemNode;

extern MemNode* memory;

MemNode* createMemNode(size_t n);
void reallocMemNode(MemNode* node,size_t capacity);
void clean_all();

#endif