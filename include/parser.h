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
    AST_IN,
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
    AST_INDEX,
    AST_LEN,
    AST_TYPE,
    AST_MOUNT,
    AST_FOR,
    AST_UPLUS,
    AST_UMINUS,
    AST_KILL,
    AST_LIST,
    AST_POP,
    AST_CHMOD
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
    struct astNode* range_start;
    struct astNode* range_end;
    struct astNode* range_skip;
    struct astNode* nextIndex;
    struct astNode* nextListElement;
    int lineCount;
    int permission_bits;
    bool isExtern;
    bool isConstant;
    bool isStringLoop;
    bool isIndexed;
    bool isVariableTraversal;
} astNode;

astNode* createAstNode();
void consume();
astNode* parseBlock();
astNode* parseMount();
astNode* parseFunction();
astNode* parseCallFunction();
astNode* parseParameters();
astNode* parseReturn();
astNode* parseIf();
astNode* parseWhileLoop();
astNode* parseForLoop();
astNode* parseVarDec();
astNode* parseIO();
astNode* parseTypeFunction();
astNode* parseExpression();
astNode* parseNot();
astNode* parseAndOr();
astNode* parseComparator();
astNode* parseEquality();
astNode* parseAddSub();
astNode* parseUnary();
astNode* parseMulDivMod();
astNode* parseAtom();
astNode* parseIndex();
astNode* parseKill();
astNode* parsePop();
astNode* parseChmod();
void Parser();

#endif
