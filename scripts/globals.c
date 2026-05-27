#include <globals.h>

void clear_states(){
    lexer_output = NULL;
    lexer_tail = NULL;
    stk = NULL;
    current = NULL;
    ast_root = NULL;
    ast_tail = NULL;
}
GlobalsCopy copy_state(){
    GlobalsCopy copy;
    copy.lexer_output = lexer_output;
    copy.lexer_tail = lexer_tail;
    copy.stk = stk;
    copy.current = current;
    copy.ast_root = ast_root;
    copy.ast_tail = ast_tail;
    return copy;
}
void restore_state(GlobalsCopy copy){
    lexer_output = copy.lexer_output;
    lexer_tail = copy.lexer_tail;
    stk = copy.stk;
    current = copy.current;
    ast_root = copy.ast_root;
    ast_tail = copy.ast_tail;
}