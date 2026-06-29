
#ifndef INDEX_H
#define INDEX_H

typedef struct Index{
    int index;
    struct Index* next;
} Index;

Index* createIndex();

#endif
