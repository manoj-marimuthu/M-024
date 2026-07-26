#include <stdio.h>
#include <memoryHandler.h>
#include <stdlib.h>
#include <error.h>

void error(char* line,int lineCount,ErrorType type){
    if(type == COMPILE_TIME_ERROR){
        printf(BOLD RED UNDERLINE "COMPILE TIME ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
    }
    else if(type == SYNTAX_ERROR){
        printf(BOLD RED UNDERLINE "RUN TIME ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check Documentation @ github for syntax \n" RESET);
    }
    else if(type == DIVISION_BY_ZERO_ERROR){
        printf(BOLD RED UNDERLINE "RUN TIME ERROR \n" RESET);
        printf(BLUE "%s \n" ,line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check for /0 and remove it \n" RESET);
    }else if(type == FILE_NOT_FOUND_ERROR){
        printf(BOLD RED UNDERLINE "FILE NOT FOUND ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        printf(GREEN "Hint : Check FileName and Extension '.app' \n" RESET);
    }else if(type == COMPILE_TIME_ERROR){
        printf(BOLD RED UNDERLINE "COMPILE TIME ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        printf(GREEN "Hint : Check Documentation @ github for Common Bugs \n" RESET);
    }else if(type == FILE_ARG_ERROR){
        printf(BOLD RED UNDERLINE "FILE ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        printf(GREEN "Hint : Only Pass Filename with Extension as Argument \n" RESET);
    }else if(type == FILE_EXTENSION_ERROR){
        printf(BOLD RED UNDERLINE "FILE ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        printf(GREEN "Hint : Only use '.app' extention for running \n" RESET);
    }else if(type ==INDENTATION_ERROR){
        printf(BOLD RED UNDERLINE "INDENTATION ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Leave spaces By Checking With Previous Lines \n" RESET);
    }
    else if(type == RUN_TIME_ERROR){
        printf(BOLD RED UNDERLINE "RUN TIME ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check Syntax @ github \n");
    }else if(type == FUNCTION_ERROR){
        printf(BOLD RED UNDERLINE "FUNCTION ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check Function Name and Whether it is already Declared or not \n" RESET);
    }else if(type == INDEXING_ERROR){
        printf(BOLD RED UNDERLINE "INDEXING ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check if index >= string length, Parameter Type (string[number] is only accepted). \n" RESET);
    }
    else if(type == IMPORT_ERROR){
        printf(BOLD RED UNDERLINE "IMPORT ERROR \n" RESET);
        printf(BLUE "%s \n",line);
        if(lineCount >= 1){
            printf(YELLOW "At line %d \n" RESET , lineCount);
        }
        printf(GREEN "Hint : Check filepath (must be relative to interpret.exe and not the parent file) and check for deadlocks. \n" RESET);
    }
    printf(RESET);
    clean_all();
    exit(1);
}