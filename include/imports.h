#ifndef IMPORTS_H
#define IMPORTS_H
#include <stdbool.h>
typedef struct Import{
    char* fileName;
    struct Import* next;
} Import;

extern Import* loaded;
extern Import* loading;
extern Import* create_import(char* fileName);
void push_import_loaded(Import* import);
void push_import_loading(Import* import);
Import* pop_import_loading();
bool isAlreadyLoaded(char* fileName);
#endif
