#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <interpret_api.h>
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
#include <direct.h>
#define VERSION "V0.4.0"

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
    if(n >= 6 && strcmp(fileName + n - 5,".mscf") == 0){
        interpret(fileName);
    }else{
        error("Invalid File Name/Extension",-1,FILE_NOT_FOUND_ERROR);
    }
    clean_all();
    return 0;
}