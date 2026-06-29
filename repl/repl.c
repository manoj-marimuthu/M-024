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
#include <direct.h>
#include <interpret_api.h>
#define REPL_FILE_NAME "__repl__.mscf"
void REPL_ON_LOAD(){
    printf(BLUE "M024 PROGRAMMING LANGUAGE\n");
    printf(GREEN "Interactive REPL environment v0.4.0 \n");
    printf(RED "Type 'exit' to quit.\n\n");
    
    printf(RESET);
}
int main(){
    REPL_ON_LOAD();
    char inp[1024] = "";
    while(strcmp(inp,"exit\n") != 0){
        printf(PURPLE ">>> " RESET);
        fgets(inp,sizeof(inp),stdin);
        if(strcmp(inp,"exit\n") == 0){
            break;
        }
        FILE* fp = fopen(REPL_FILE_NAME,"w");
        fputs(inp,fp);
        fclose(fp);
        interpret(REPL_FILE_NAME);
        lexer_output = NULL;
        lexer_tail = NULL;
        ast_root = NULL;
        ast_tail = NULL;
        stk = NULL;
        current = NULL;
    }
    clean_all();
    return 0;
}