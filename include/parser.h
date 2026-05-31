#ifndef PARSER_H
#define PARSER_H 

#include <memoryHandler.h>
#include <lexer.h>

typedef enum{
    AST_ROOT,
    AST_NUMBER,
    AST_BOOLEAN,
    AST_STRING,
    AST_PRINT,
    AST_INPUT,
    AST_OPERATOR,
    AST_AND,
    AST_NONE,
    AST_OR,
    AST_NOT,
    AST_IF,
    AST_ELSE,
    AST_WHILE,
    AST_INT,
    AST_STR,
    AST_BOOL,
    AST_IDENTIFIER,
    AST_EQ,
    AST_NEQ,
    AST_LT,
    AST_GT,
    AST_LEQ,
    AST_GEQ,
    AST_FUNCTION_DECLARATION,
    AST_FUNCTION_CALL,
    AST_RETURN,
    AST_ASSIGNMENT,
    AST_INDEXED,
    AST_IDENTIFIER_INDEXED,
    AST_LEN,
    AST_TYPE,
    AST_LOAD,
    AST_FOR
} astNodeType;

typedef struct astNode{
    astNodeType type;
    union{
        double numData;
        char* stringData;
        bool boolData;
        char charData;
    }   data;
    int param_length;
    struct astNode* nextBlock;
    struct astNode* thenBlock;
    struct astNode* thenNext;
    struct astNode* elseBlock;
    struct astNode* left;
    struct astNode* right;
    struct astNode* child;
    struct astNode* param;
    struct astNode* nextParam;
    struct astNode* astChain;
    struct astNode* index;
    struct astNode* start_value;
    struct astNode* end_value;
    int lineCount;
    bool isConstant;
} astNode;

astNode* createAstNode();
void consume();
astNode* parseBlock();
astNode* parseLoad();
astNode* parseFunction();
astNode* parseCallFunction();
astNode* parseParameters();
astNode* parseReturn();
astNode* parseIf();
astNode* parseLoop();
astNode* parseVarDec();
astNode* parseIO();
astNode* parseTypeFunction();
astNode* parseExpression();
astNode* parseNot();
astNode* parseAndOr();
astNode* parseComparator();
astNode* parseAddSub();
astNode* parseMulDivMod();
astNode* parseAtom();
void Parser();

#endif