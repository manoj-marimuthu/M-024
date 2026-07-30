#include <interpret_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <lexer.h>
#include <indentation.h>
#include <parser.h>
#include <memoryHandler.h>
#include <evaluate.h>
#include <variableRegistry.h>
#include <callStack.h>
#include <globals.h>
#include <imports.h>
#define VERSION "V0.4.0"

void interpret(char* fileName){
    int n = strlen(fileName);
    if(n > 5 && fileName[n-1] == 'f' && fileName[n-2] == 'c' && fileName[n-3] == 's' && fileName[n-4] == 'm' && fileName[n-5] == '.'){
        CallStackNode* global_frame = createCallStackNode();
        pushCallStackNode(global_frame);
        Import* parent_import = create_import(fileName);
        push_import_loading(parent_import);
        Lexer(fileName);
	MemNode* curFileNameObj = createMemNode(strlen(fileName)+1);
	strcpy(curFileNameObj->ptr,fileName);
	curFileName = curFileNameObj->ptr;
        current = lexer_output;
        Parser();
        Execute();
        Import * imp = pop_import_loading();
        push_import_loaded(imp);
    }else{
        error("File Extension Is Wrong",-1,FILE_EXTENSION_ERROR);
    }
}
