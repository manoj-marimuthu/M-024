#include <parser.h>
#include <memoryHandler.h>
#include <error.h>
#include <lexer.h>
#include <string.h>
#include <evaluate.h>
#include <globals.h>
#include <variableRegistry.h>
void skipNewline(){
    while(current && current->type == NEWLINE) consume();
}
astNode* createAstNode(){
    MemNode* node = createMemNode(sizeof(astNode));
    astNode* temp = node->ptr;
    temp->type = AST_NONE;
    temp->left = NULL;
    temp->right = NULL;
    temp->child = NULL;
    temp->thenNext = NULL;
    temp->nextBlock = NULL;
    temp->thenBlock = NULL;
    temp->elseBlock = NULL;
    temp->astChain = NULL;
    temp->param_length = 0;
    temp->index = 0;
    temp->lineCount = current->lineCount;
    temp->param = NULL;
    temp->nextParam = NULL;
    temp->isConstant = false;
    return temp;
}
void consume(){
    if(current != NULL){
        current = current->next;
    }
}
astNode* parseAtom(){
    if(current && current->type == NUMBER){
        astNode* node = createAstNode();
        node->type = AST_NUMBER;
        node->data.numData = current->data.numData;
        consume();
        return node;
    }
    else if(current && current->type == STRING){
        astNode* node = createAstNode();
        node->type = AST_STRING;
        MemNode* gcObj = createMemNode(strlen(current->data.strData)+1);
        node->data.stringData = gcObj->ptr;
        strcpy(node->data.stringData,current->data.strData);    
        consume();
        if(current && current->type == L_SQUARE_BRACK){
            consume();
            astNode* indexNode = parseExpression();
            if(current && current->type == R_SQUARE_BRACK){
                consume();
                node->index = indexNode;
                node->type = AST_INDEXED;
                return node;
            }else{
                error("Missing ] for indexing",current->lineCount,SYNTAX_ERROR);
            }
        }
        return node;
    }
    else if(current && current->type == BOOLEAN){
        astNode* node = createAstNode();
        node->type = AST_BOOLEAN;
        node->data.boolData = current->data.boolData;
        consume();
        return node;
    }
    else if(current && current->type == NONE){
        astNode* node = createAstNode();
        node->type  = AST_NONE;
        consume();
        return node;
    }
    else if(current->type == IDENTIFIER && current->next->type == L_BRACK) return parseCallFunction();
    else if(current && current->type == IDENTIFIER){
        astNode* node = createAstNode();
        node->type = AST_IDENTIFIER;
        MemNode* gcObj = createMemNode(strlen(current->data.strData)+1);
        node->data.stringData = gcObj->ptr;
        strcpy(node->data.stringData,current->data.strData);
        consume();
        if(current && current->type == L_SQUARE_BRACK){
            consume();
            astNode* indexNode = parseExpression();
            if(current && current->type == R_SQUARE_BRACK){
                consume();
                node->index = indexNode;
                node->type = AST_IDENTIFIER_INDEXED;
                return node;
            }else{
                error("Missing ] while indexing a variable",current->lineCount,SYNTAX_ERROR);
            }
        }
        return node;
    }
    else if(current && current->type == L_BRACK){
        consume();
        if(current == NULL){
            error("Unexpected Error While Parsing",current->lineCount,RUN_TIME_ERROR);
        }
        astNode* node = parseExpression();
        if(current->type != R_BRACK){
            error("Missing ')' in Expression",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        return node;
    }
    else if(current && (current->type == INPUT || current->type == PRINT)){
        return parseIO();
    }
    else if(current && (current->type == INT || current->type ==  STR || current->type == BOOL || current->type == LEN || current->type == TYPE)){
        return parseTypeFunction();
    }
    else{
        error("Undefined Error While Parsing (Check Indentations/Logic)\n",current->lineCount,RUN_TIME_ERROR);
        return NULL;
    }
}

astNode* parseExponent(){
    astNode* lhs = parseAtom();
    while(current != NULL && (current->type != TO && current->type != R_BRACK && current->type != DEDENT && current->type != COMMA && current->type != NEWLINE ) && current->type == OPERATOR && (current->data.charData == '^')){
        astNode* parent = createAstNode();
        parent->data.charData = '^';
        parent->type = AST_OPERATOR;
        consume();
        astNode* rhs = parseAtom();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}
astNode* parseAddSub(){
    astNode* lhs = parseMulDivMod();
    while(current != NULL && (current->type != TO && current->type != R_BRACK && current->type != DEDENT && current->type != COMMA && current->type != NEWLINE ) && current->type == OPERATOR && (current->data.charData == '+' || current->data.charData == '-')){
        astNode* parent = createAstNode();
        parent->type = AST_OPERATOR;
        parent->data.charData = current->data.charData;
        consume();
        astNode* rhs = parseMulDivMod();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}
astNode* parseMulDivMod(){
    astNode* lhs = parseExponent();
    if(lhs == NULL) error("Undefined Error While Evaluating Expression",current->lineCount,RUN_TIME_ERROR);
    while(current != NULL && (current->type != TO && current->type != R_BRACK && current->type != DEDENT && current->type != COMMA && current->type != NEWLINE ) && current->type == OPERATOR && (current->data.charData == '*' || current->data.charData == '/' || current->data.charData == '%')){
        astNode* parent = createAstNode();
        parent->type = AST_OPERATOR;
        parent->data.charData = current->data.charData;
        consume();
        astNode* rhs = parseExponent();
        if(rhs == NULL) error("Undefined Error While Evaluating Expression",current->lineCount,RUN_TIME_ERROR);
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}

astNode* parseComparator(){
    astNode* lhs = parseAddSub();
    if(lhs == NULL) error("Unexpected Error While Parsing Comparators",current->lineCount,RUN_TIME_ERROR);
    while(current != NULL && (current->type != TO && current->type != R_BRACK && current->type != DEDENT && current->type != COMMA && current->type != NEWLINE ) && (current->type == EQ || current->type == NEQ || current->type == GT || current->type == LT || current->type == GEQ || current->type == LEQ)){
        astNode* parent = createAstNode();
        switch(current->type){
            case EQ:parent->type = AST_EQ;break;
            case NEQ:parent->type = AST_NEQ;break;
            case GT:parent->type = AST_GT;break;
            case LT:parent->type = AST_LT;break;
            case GEQ:parent->type = AST_GEQ;break;
            case LEQ:parent->type = AST_LEQ;break;
            default:break;
        }
        consume();
        astNode* rhs = parseAddSub();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}

astNode* parseAndOr(){
    astNode* lhs = parseComparator();
    if(lhs == NULL) error("Unexpected Error While Parsing",lhs->lineCount,RUN_TIME_ERROR);
    while(current != NULL && (current->type != TO && current->type != R_BRACK && current->type != DEDENT && current->type != COMMA && current->type != NEWLINE ) && (current->type == AND || current->type == OR)){
        astNode* parent=  createAstNode();
        if(current->type == AND) parent->type = AST_AND;
        else parent->type = AST_OR;
        consume();
        astNode* rhs = parseComparator();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}
astNode* parseIn(){
    astNode* lhs = parseAndOr();
    if(current && current->type == IN){
        consume();
        astNode* rhs = parseAndOr();
        astNode* in_node = createAstNode();
        in_node->left = lhs;
        in_node->right = rhs;
        in_node->type = AST_IN;
        return in_node;
    }
    return lhs;
}
astNode* parseNot(){
    if(current && current->type == NOT){
        astNode* parent = createAstNode();
        parent->type = AST_NOT;
        consume();
        astNode* child = parseNot();
        parent->child = child;
        return parent;
    }
    return parseIn();
}

astNode* parseExpression(){
    return parseNot();
}

astNode* parseIO(){
    if(current && current->type == PRINT){
        consume();
        if(current && current->type != L_BRACK){
            error("Missing '(' for Built-in print() function",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        astNode* printNode = createAstNode();
        printNode->type = AST_PRINT;
        astNode* head = NULL;
        astNode* tail = NULL;
        while(current && current->type != R_BRACK){
            astNode* toPrint = parseExpression();
            if(head == NULL){
                head = toPrint;
                tail = toPrint;
            }else{
                tail->thenNext = toPrint;
                tail = toPrint;
            }
            if(current->type == COMMA) consume();
        }
        printNode->child = head;
        if(current && current->type != R_BRACK){
            error("Missing ')' for Built-in print() function",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        skipNewline();
        if(printNode->child == NULL){
            error("print() requires parameters,0 parameters given",current->lineCount,SYNTAX_ERROR);
        }
        
        return printNode;
    }else if(current && current->type == INPUT){
        consume();
        if(current->type != L_BRACK){
            error("Missing '(' for Built-in input() function",current->lineCount,SYNTAX_ERROR);
        }
        astNode* inputNode = createAstNode();
        inputNode->type = AST_INPUT;
        consume();
        astNode* inputAsker=  parseExpression();
        inputNode->child = inputAsker;
        if(current && current->type != R_BRACK){
            error("Missing ')' for Built-in input() function",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        skipNewline();
        return inputNode;
    }else{
        return NULL;
    }
}
astNode* parseTypeFunction(){
    if(current && current->type == INT){
        astNode* node = createAstNode();
        node->type = AST_INT;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* child = parseExpression();
            node->child = child;
            if(current && current->type == R_BRACK){
                consume();
                skipNewline();
                return node;
            }else{
                error("Missing ')' in int() built-in function",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Missing '(' in int() built-in function",current->lineCount,SYNTAX_ERROR);
        }
    }
    else if(current && current->type == STR){
        astNode* node = createAstNode();
        node->type = AST_STR;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* child = parseExpression();
            node->child = child;
            if(current && current->type == R_BRACK){
                consume();
                if(current && current->type == NEWLINE){
                    consume();
                }
            return node;
            }else{
                error("Missing ')' in str() built-in function",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Missing '(' in str() built-in function",current->lineCount,SYNTAX_ERROR);
        }
    }
    else if(current && current->type == BOOL){
        astNode* node = createAstNode();
        node->type = AST_BOOL;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* child = parseExpression();
            node->child = child;
            if(current && current->type == R_BRACK){
                consume();
                skipNewline();
                return node;
            }else{
                error("Missing ')' in bool() built-in function",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Missing '(' in bool() built-in function",current->lineCount,SYNTAX_ERROR);
        }
    }
    else if(current && current->type == LEN){
        astNode* node = createAstNode();
        node->type = AST_LEN;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* child = parseExpression();
            node->child = child;
            if(current && current->type == R_BRACK){
                consume();
                skipNewline();
                return node;
            }
            else{
                error("Missing ')' in len() built-in function",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Missing '(' in len() built-in function",current->lineCount,SYNTAX_ERROR);
        }
    }
    else if(current && current->type == TYPE){
        astNode* node = createAstNode();
        node->type = AST_TYPE;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* child = parseExpression();
            node->child = child;
            if(current && current->type == R_BRACK){
                consume();
                skipNewline();
                return node;
            }
            else{
                error("Missing ')' in type() built-in function",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Missing '(' in type() built-in function",current->lineCount,SYNTAX_ERROR);
        }
    }
    else{
        return parseIO();
    }
    return NULL;
}

astNode* parseVarDec(){
    if(current && (current->type == IDENTIFIER || current->type == CONST)){
        bool isConstant = false;
        if(current && current->type == CONST) {isConstant = true;consume();}
        if(current && current->type != IDENTIFIER) error("Using 'final' Keyword Without Identifier",current->lineCount,SYNTAX_ERROR);
        astNode* node = createAstNode();
        astNode* name = createAstNode();
        node->type = AST_ASSIGNMENT;
        name->type = AST_STRING;
        MemNode* gcObj = createMemNode(strlen(current->data.strData)+1);
        char* ptr = gcObj->ptr;
        name->data.stringData = ptr;
        strcpy(name->data.stringData,current->data.strData);
        node->left = name;
        consume();
        if(current && current->type != EQUALS){
            error("Missing '=' in a variable/constant declaration",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        astNode* value = parseExpression();
        node->right = value;
        node->isConstant = isConstant;
        skipNewline();
        return node;
    }else{
        return NULL;
    }
}

astNode* parseLoop(){
    if(current && current->type == WHILE){
        consume();
        astNode* whileNode = createAstNode();
        astNode* condition = parseExpression();
        whileNode->child = condition;
        whileNode->type = AST_WHILE;
        if(current && current->type != COLON){
            error("Missing ':' in while loop declaration",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        skipNewline();
        if(current && current->type != INDENT){
            error("Missing Indentation in While Loop",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        astNode* head = NULL;
        astNode* tail = NULL;
        while(current && current->type != DEDENT){
            astNode* statement = parseBlock();
            if(statement == NULL) break;
            if(head == NULL){
                head = statement;
                tail = statement;
            }else{
                tail->thenNext = statement;
                tail = statement;
            }
            skipNewline();
        }
        if(current && current->type != DEDENT){
            error("Indentation Does Not End Properly",current->lineCount,INDENTATION_ERROR);
        }
        consume();
        whileNode->thenBlock = head;
        return whileNode;
    }
    else if(current && current->type == FOR){
        consume();
        if(current && current->type == IDENTIFIER){
            char* varName = current->data.strData;
            consume();
            if(current && current->type == EQUALS){
                // for loop for a defined range
                consume();
                astNode* start_value = parseExpression();
                if(current && current->type  == TO){
                    consume();
                    astNode* end_value = parseExpression();
                    if(current && current->type == COLON){
                        consume();
                        skipNewline();
                        if(current && current->type == INDENT){
                            consume();
                            astNode* node = createAstNode();
                            node->type = AST_FOR;
                            MemNode* strObj = createMemNode(strlen(varName) + 1);
                            strcpy(strObj->ptr,varName);
                            node->data.stringData = varName;
                            astNode* head = NULL;
                            astNode* tail = NULL;
                            while(current && current->type != DEDENT){
                                astNode* stmt = parseBlock();
                                if(head == NULL){
                                    head = stmt;
                                    tail = stmt;
                                }else{
                                    tail->thenNext = stmt;
                                    tail = stmt;
                                }
                                skipNewline();
                            }
                            if(current && current->type != DEDENT){
                                error("Indentation Does Not End Properly",current->lineCount,INDENTATION_ERROR);
                            }
                            consume();
                            skipNewline();
                            node->thenBlock = head;
                            node->start_value = start_value;
                            node->end_value = end_value;
                            return node;
                        }else{
                            error("Indentation not found for 'for loop'",current->lineCount,SYNTAX_ERROR);
                        }
                    }else{
                        error("invalid syntax , ':' (colon) required in the for loop",current->lineCount,SYNTAX_ERROR);
                    }
                }else{
                    error("invalid syntax for 'for loop', requires range end",current->lineCount,SYNTAX_ERROR);
                }

            }else if(current && current->type == IN){
                // for loop for a string
                consume();
            }else{
                error("for loop requires '=' for number ranges or 'in' for string traversal to work",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("for loop requires an identifier (variable)",current->lineCount,SYNTAX_ERROR);
            return NULL;
        }
    }
    return NULL;
}

astNode* parseIf(){
    if(current && (current->type == IF || current->type == ELIF)){
        consume();
        astNode* condition = parseExpression();
        if(current && current->type == R_BRACK) consume();
        if(current && current->type != COLON){
            error("Missing ':' in if statement",current->lineCount,SYNTAX_ERROR);
        }
        consume();
        skipNewline();
        if(current && current->type != INDENT){
            error("Indentation Does Not Start Correctly",current->lineCount,INDENTATION_ERROR);
        }
        consume();
        astNode* ifNode = createAstNode();
        ifNode->type = AST_IF;
        ifNode->child = condition;
        astNode* head = NULL;
        astNode* tail = NULL;   
        while(current && current->type != DEDENT){
            astNode* statement = parseBlock();
            if(head == NULL){
                head = statement;
                tail = statement;
            }else{
                tail->thenNext = statement;
                tail = statement;
            }
           skipNewline();
        }
        ifNode->thenBlock = head;
        if(current && current->type !=DEDENT){
            error("Indentation Does not End Properly",current->lineCount,INDENTATION_ERROR);
        }
        if(current && current->type == DEDENT) consume();
        skipNewline();
        if(current && current->type == ELIF){
            ifNode->nextBlock  = parseIf();
        }
        else if(current && current->type == ELSE){
            consume();
            if(!current || current->type != COLON){
                error("Missing ':' in else statement",current->lineCount,SYNTAX_ERROR);
            }
            consume();
            skipNewline();
            if(current && current->type != INDENT){
                error("Indentation Does not Start Properly",current->lineCount,INDENTATION_ERROR);
            }
            consume();
            astNode* elseNode = createAstNode();
            elseNode->type = AST_ELSE;
            astNode* head = NULL;
            astNode* tail = NULL;
            while(current && current->type != DEDENT){
                astNode* statement = parseBlock();
                 if(statement == NULL) break;
                skipNewline();
                if(head == NULL){
                    head = statement;
                    tail = statement;
                }else{
                    tail->thenNext = statement;
                    tail = statement;
                }
            }
            elseNode->thenBlock = head;
            if(current && current->type !=DEDENT){
                error("Indentation Does Not End Properly",current->lineCount,INDENTATION_ERROR);
            }
            if(current && current->type == DEDENT) consume();
            if(current && current->type == NEWLINE) consume();
            ifNode->elseBlock = elseNode;
        }
        return ifNode;
    }else{
        return NULL;
    }
}
astNode* parseParameters(){
    astNode* params = NULL;
    astNode* tail = NULL;
    while(current && current->type != R_BRACK){
        if(current && current->type == IDENTIFIER){
            astNode* param = parseExpression(); 
            if(params  == NULL){
                params = param;
                tail = param;
            }else{
                tail->nextParam = param;
                tail = param;
            }
            params->param_length++;
        }
        if(current->type == COMMA){
            consume();
        }
    }
    return params;
}
astNode* parseReturn(){
    if(current && current->type == RETURN){
        astNode* returnNode = createAstNode();
        returnNode->type = AST_RETURN;
        consume();
        astNode* expr = parseExpression();
        returnNode->child = expr;
        skipNewline();
        return returnNode;
    }else{
        return NULL;
    }
}
astNode* parseFunction(){
    if(current && current->type == FUNCTION){
        consume();
        astNode* node = createAstNode();
        node->type = AST_FUNCTION_DECLARATION;
        if(current && current->type == IDENTIFIER){
            MemNode* strObj = createMemNode(strlen(current->data.strData)+1);
            strcpy(strObj->ptr,current->data.strData);
            node->data.stringData = strObj->ptr;
            consume();
            if(current && current->type != L_BRACK){
                error("Wrong Syntax For Declaration of a Function , '(' is Mandatory",current->lineCount,SYNTAX_ERROR);
            }else{
                consume();
                node->param = parseParameters();
                if(current && current->type == R_BRACK){
                    consume();
                    if(current && current->type == COLON){
                        consume();
                        skipNewline();
                        if(current && current->type == INDENT){
                            consume();
                            astNode* head = NULL;
                            astNode* tail = NULL;
                            while(current &&  current->type != DEDENT){
                                astNode* stmt = parseBlock();
                                 if(stmt == NULL) break;
                               skipNewline();
                                if(head == NULL){
                                    head = stmt;
                                    tail = stmt;
                                }else{
                                    tail->thenNext = stmt;
                                    tail = stmt;
                                }
                               skipNewline();
                            }
                            node->thenBlock = head;
                            if(current->type == DEDENT) consume();
                            return node;
                        }else{
                            error("Indentation Error at function declaration",current->lineCount,INDENTATION_ERROR);
                        }
                    }else{
                        error("Missing ':' in function declaration",current->lineCount,SYNTAX_ERROR);
                    }
                }else{
                    error("Missing ')' in function declaration",current->lineCount,SYNTAX_ERROR);
                }
            }
        }else{
            error("Usage of Built in keyword 'fn' is prohibited",current->lineCount,SYNTAX_ERROR);
        }
    }else{
        error("Undefined Statement Found By Parser (while parsing function)",current->lineCount,RUN_TIME_ERROR);
        return NULL;
    }
    return NULL;
}
astNode* parseCallFunction(){
    if(current && current->type == IDENTIFIER && current->next->type == L_BRACK){
        astNode* node = createAstNode();
        node->type = AST_FUNCTION_CALL;
        MemNode* strObj = createMemNode(strlen(current->data.strData) + 1);
        strcpy(strObj->ptr,current->data.strData);
        node->data.stringData = strObj->ptr;
        consume();
        if(current && current->type == L_BRACK){
            consume();
            astNode* head = NULL;
            astNode* tail = NULL;
            while(current && current->type != R_BRACK){
                astNode* param = parseExpression();
                if(head == NULL){
                    head = param;
                    tail = param;
                }else{
                    tail->nextParam = param;
                    tail = param;
                }
                if(current->type == COMMA) consume();
                node->param_length++;
            } 
            node->param = head;
            if(current && current->type == R_BRACK){
                consume();
                return node;
            }else{
                error("Call Function is Missing ')'",current->lineCount,SYNTAX_ERROR);
            }
        }else{
            error("Call Function is Missing '('",current->lineCount,SYNTAX_ERROR);
        }
    }
    return NULL;
}
astNode* parseLoad(){
    skipNewline();
    if(current && current->type == LOAD){
        consume();
        if(current->type == STRING){
            astNode* node = createAstNode();
            node->type = AST_LOAD;
            MemNode* strObj = createMemNode(strlen(current->data.strData) + 1);
            strcpy(strObj->ptr,current->data.strData);
            node->data.stringData = strObj->ptr;
            consume();
            return node;
        }
        else{
            error("Unidentified object located after 'load' keyword. Use a valid string which denotes a filename",current->lineCount,SYNTAX_ERROR);
            return NULL;
        }
    }else{
        return NULL;
    }
}
astNode* parseBlock(){
    skipNewline();
    LexerNode* node = current;
    if(node == NULL){
        error("Undefined Error While Parsing",current->lineCount,RUN_TIME_ERROR);
    }
    if(node->type == PRINT  || node->type == INPUT) return parseIO();
    else if(node->type == IF) return parseIf();
    else if(node->type == WHILE || node->type == FOR) return parseLoop();
    else if(node->type == IDENTIFIER && node->next->type == L_BRACK) return parseCallFunction();
    else if(node->type == IDENTIFIER || node->type == CONST) return parseVarDec();
    else if(node->type == FUNCTION) return parseFunction();
    else if(node->type == RETURN) return parseReturn();
    else if(node->type == LOAD) return parseLoad();
    else{ 
        error("Undefined Statement Found While Parsing",current->lineCount,RUN_TIME_ERROR);
        return NULL;
    }
}

void Parser(){
    while(current){
         skipNewline();
        astNode* node = parseBlock();
        if(node == NULL){
            if(current && current->type == DEDENT){
                consume();
                continue;
            }
            error("Unexpected Error While Parsing",current->lineCount,RUN_TIME_ERROR);
        }
        if(ast_root == NULL){
            ast_root = node;
            ast_tail = node;
        }else{
            ast_tail->astChain = node;
            ast_tail = node;
        }
        skipNewline();
    }
}
