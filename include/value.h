#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
typedef struct List List;
typedef enum {
    D_STRING,
    D_NUMBER,
    D_BOOLEAN,
    D_NONE,
    D_SKIP
} DataType;

typedef struct Value{
    DataType type;
    union{
        char* stringData;
        double numData;
        bool boolData;
        List* listData;
    } data;
    bool isReturnedValue;
} Value;

#endif