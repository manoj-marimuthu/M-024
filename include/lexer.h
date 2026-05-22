#ifndef LEXER_H
#define LEXER_H
#include <indentation.h>
#include <stdbool.h>
typedef enum{
    INCLUDE,
    STRING,
    NUMBER,
    BOOLEAN,
    AND,
    OR,
    NOT,
    OPERATOR,
    IDENTIFIER,
    EQUALS,
    EQ,
    NEQ,
    GEQ,
    LEQ,
    GT,
    LT,
    COLON,
    NEWLINE,
    INDENT,
    DEDENT,
    L_BRACK,
    R_BRACK,
    FOR,
    TO,
    COMMA,
    PRINT,
    INPUT,
    INT,
    STR,
    BOOL,
    WHILE,
    IF,
    ELIF,
    ELSE,
    CONST,
    NONE,
    RETURN,
    FUNCTION
} LexerNodeType;

typedef struct LexerNode
{
    LexerNodeType type;
    union{
        double numData;
        char charData;
        char* strData;
        bool boolData;
    }  data; 
    int lineCount;
    struct LexerNode* next;
} LexerNode;

LexerNode* lexer_output;
LexerNode* lexer_tail;
Stack* stk;
void Lexer(char* fileName);
void pushIntoLexerOutput(LexerNode* node);
void line_lexer(char* line,int lineCount,Stack* stk);

#endif