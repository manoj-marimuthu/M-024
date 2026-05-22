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
#define VERSION "V0.2.0"

void interpret(char* fileName){
    int n = strlen(fileName);
    if(n > 4 && fileName[n-1] == 'p' && fileName[n-2] == 'p' && fileName[n-3] == 'a' && fileName[n-4] == '.'){
        CallStackNode* global_frame = createCallStackNode();
        pushCallStackNode(global_frame);
        Lexer(fileName);
        current = lexer_output;
        Parser();
        Execute();
    }else{
        error("File Extension Is Wrong",-1,FILE_EXTENSION_ERROR);
    }
}

int main(int argCount,char* args[]){
    if(argCount > 2){
        error("Passing Multiple Arguments is Illegal",-1,FILE_ARG_ERROR);
    }
    if(argCount < 2){
        error("Passing No Arguments is Illegal",-1,FILE_ARG_ERROR);
    }
    char* fileName = args[1];
    int n =strlen(fileName);
    if(strcmp(fileName,"--version") == 0 || strcmp(fileName,"--v") == 0){
        printf(VERSION);
        clean_all();
        exit(1);
    }
    if(n >= 4 && strcmp(fileName + n - 4,".app") == 0){
        interpret(fileName);
    }else{
        error("Invalid File Name/Extension",-1,FILE_NOT_FOUND_ERROR);
    }
    clean_all();
    return 0;
}