#include <value.h>
#include <memoryHandler.h>

// handle carefully, garbage values while Value was created must be handled

Value* createValue(){
    MemNode* valObj = createMemNode(sizeof(Value));
    return valObj->ptr;
}