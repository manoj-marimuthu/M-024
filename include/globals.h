#ifndef GLOBALS_H
#define GLOBALS_H
#include <lexer.h>
#include <parser.h>
#include <evaluate.h>
#include <indentation.h>

// for storing and retrieving states of the interpreter
typedef struct GlobalsCopy{
    LexerNode* lexer_output;
    LexerNode* lexer_tail;
    Stack* stk;
    LexerNode* current;
    astNode* ast_root;
    astNode* ast_tail;
} GlobalsCopy;

// actual globals used in files
extern LexerNode* lexer_output;
extern LexerNode* lexer_tail;
extern Stack* stk;
extern LexerNode* current;
extern astNode* ast_root;
extern astNode* ast_tail;

void clear_states();
GlobalsCopy copy_state();
void restore_state(GlobalsCopy copy);

#endif
