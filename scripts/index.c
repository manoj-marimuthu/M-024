#include <index.h>
#include <memoryHandler.h>

Index* createIndex(){
    MemNode* indexObj = createMemNode(sizeof(Index));
    Index* index = indexObj->ptr;
    index->index = -1;
    index->next = NULL;
    return index;
}