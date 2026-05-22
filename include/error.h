#ifndef ERROR_H
#define ERROR_H
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define RESET       "\033[0m"
typedef enum{
    SYNTAX_ERROR,
    DIVISION_BY_ZERO_ERROR,
    COMPILE_TIME_ERROR,
    FILE_NOT_FOUND_ERROR,
    FILE_ARG_ERROR,
    FILE_EXTENSION_ERROR,
    INDENTATION_ERROR,
    RUN_TIME_ERROR,
    FUNCTION_ERROR,
    INDEXING_ERROR
} ErrorType;
void error(char* line,int lineCount,ErrorType type);
#endif