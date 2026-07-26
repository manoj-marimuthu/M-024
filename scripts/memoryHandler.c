#include <memoryHandler.h>
#include <error.h>
#include <variableRegistry.h>
#include <stdlib.h>

MemNode* memory = NULL;
MemNode* createMemNode(size_t n){
    MemNode* node = malloc(sizeof(MemNode));
    if(!node){
        error("Memory Allocation Failed in Memory Handler",0,COMPILE_TIME_ERROR);
    }
    node->next = memory;
    node->ptr = malloc(n);
    if(node->ptr == NULL){
        error("Memory Allocation Failed in Memory Handler",0,COMPILE_TIME_ERROR);
    }
    if(memory){
        memory->prev = node;
    }
    memory = node;
    return node;
}

void reallocMemNode(MemNode* node,size_t capacity){
    void* ptr = node->ptr;
    void* temp = realloc(ptr,capacity);
    if(!temp){
        error("Memory Allocation Failed in Memory Handler During Resizing",0,COMPILE_TIME_ERROR);
    }
    node->ptr = temp;
}
void clean_all(){
    MemNode* current = memory;
    while(current != NULL){
        if(current->ptr){
            free(current->ptr);
        }
        MemNode* temp = current->next;
        free(current);
        current = temp;
    }
}