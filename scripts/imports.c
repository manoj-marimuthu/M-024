#include <imports.h>
#include <stdio.h>
#include <string.h>
#include <memoryHandler.h>
#include <error.h>

Import* loaded = NULL;
Import* loading = NULL;

Import* create_import(char* fileName){
    MemNode* importObj = createMemNode(sizeof(Import));
    Import* import = importObj->ptr;
    MemNode* strObj = createMemNode(strlen(fileName) + 1);
    strcpy(strObj->ptr,fileName);
    import->fileName = strObj->ptr;
    import->next = NULL;
    return import;
}

void push_import_loaded(Import* import){
    import->next = loaded;
    loaded = import;
}
void push_import_loading(Import* import){
    Import* curImport = loading;
    while(curImport != NULL){
        if(strcmp(curImport->fileName,import->fileName) == 0){
            char errMSG[100];
            snprintf(errMSG,sizeof(errMSG),"Import Deadlock detected while importing %s",import->fileName);
            error(errMSG,-1,IMPORT_ERROR);
        }
        curImport = curImport->next;
    }
    import->next = loading;
    loading = import;
}
Import* pop_import_loading(){
    Import* toReturn = loading;
    if(loading) loading = loading->next;
    return toReturn;
}

bool isAlreadyLoaded(char* fileName){
    Import* curImport = loaded;
    while(curImport != NULL){
        if(strcmp(curImport->fileName,fileName) == 0){
           return true;
        }
        curImport = curImport->next;
    }
    return false;
}