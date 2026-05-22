#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
typedef struct MemNode{
    void* ptr;
    struct MemNode* next;
} MemNode;

MemNode* Allocations = NULL;

void freeAlloc(){
    MemNode* current = Allocations;
    while(current != NULL){
        MemNode* next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
    Allocations = NULL;
}
void error(const char* err){
    printf("Python : %s",err);
    freeAlloc();
    exit(1);
}
void* Alloc(size_t n){
    void* ptr = malloc(n);
    if(ptr == NULL) error("Memory Allocation Failed \n");
    MemNode* node = (MemNode*) malloc(sizeof(MemNode));
    if(node == NULL) error("Memory Allocation Failed \n");
    node->ptr = ptr;
    node->next = Allocations;
    Allocations = node;
    return ptr;
}
void* Realloc(void* ptr,size_t n){
    MemNode* current = Allocations;
    void* temp = realloc(ptr,n);
    if(temp == NULL) error("Memory Allcation Failed \n");
    while(current != NULL){
        if(current->ptr == ptr){
            free(current->ptr);
            current->ptr = temp;
            break;
        }
        current = current->next;
    }
    return temp;
}

void pushAlloc(void* ptr){
     MemNode* node = (MemNode*) malloc(sizeof(MemNode));
    if(node == NULL) error("Memory Allcation Failed \n");
    node->ptr = ptr;
    node->next = Allocations;
    Allocations = node;
}
typedef struct StackNode{
    int value;
    struct StackNode* next; 
} StackNode;

typedef struct {
    int length;
    StackNode* top;
} Stack;

Stack* initStack(){
    Stack* s = Alloc(sizeof(Stack));
    s->length = 0;
    s->top = NULL;
    return s;
}
void pushStack(Stack* s,int value){
    if(s == NULL) error("Python : Stack is Undefined \n");
    StackNode* newTop = (StackNode*) Alloc(sizeof(StackNode));
    newTop->next = s->top;
    newTop->value = value;
    s->top = newTop;
    s->length++;
}
int popStack(Stack* s){
    if(s == NULL) error("Python : Stack is Undefined \n");
    if(s->top == NULL){
        error("Python : Stack Underflow (maybe check Indentation ?) \n");
        freeAlloc();
        exit(1);
    }else{
        int popped = s->top->value;
        StackNode* newTop = s->top->next;
        s->top = newTop;
        s->length--;
        return popped;
    }
}

void traverseStack(Stack* s){
    StackNode* current= s->top;
    while(current != NULL){
        printf("%d \n",current->value);
        current = current->next;
    }
}
typedef enum{
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
    INDENT,
    DEDENT,
    NEWLINE,
    L_PAREN,
    R_PAREN,
    COMMA,
    PRINT,
    INPUT,
    INT,
    WHILE,
    IF,
    ELIF,
    ELSE,
    CONST,
    NONE
} LexerNodeType;

typedef struct LexerNode{
    LexerNodeType type;
    union{
        double numData;
        char charData;
        char* stringData;
        bool boolData;
    }data;
    struct LexerNode* next;
} LexerNode;
void pushLexerNode(LexerNode** head,LexerNode** tail,LexerNode* node){
    if(*head == NULL){
        *head = node;
        *tail = node;
    }else{
        (*tail)->next = node;
        *tail = node;
    }
}
Stack* s = NULL;
LexerNode* Lexer(char* line){
    int n = strlen(line);
    int i =0;
    int space = 0;
    LexerNode* head = NULL;
    LexerNode* tail = NULL;
    while(isspace(line[i])){
        space++;
        i++;
    }
    if (s == NULL || s->top == NULL) {
        error("Python: Indentation stack not initialized properly\n");
    }
    if(space > s->top->value){
        LexerNode* node = (LexerNode*) Alloc(sizeof(LexerNode));
        node->type = INDENT;
        node->next = NULL;
        pushStack(s,space);
        space =0;
        pushLexerNode(&head,&tail,node);
    }
    else if(space < s->top->value){
        if(s->top == NULL){
            error("Indentation Error \n");
        }
        bool matched = false;
        while(space < s->top->value){
            LexerNode* node = (LexerNode*) Alloc(sizeof(LexerNode));
            node->next = NULL;
            node->type = DEDENT;
            if(s->top == NULL) error("Unexpected Dedent\n");
            int popped = popStack(s);
            pushLexerNode(&head,&tail,node);
            if(s->top != NULL && s->top->value == space){
                matched= true;
                break;
            }
        }    
        if(!matched  && (s->top == NULL  || s->top->value != space)){
            error("Indentation Error \n");
        }
        space = 0;
    }
    while(i < n){
        if(isspace(line[i])){
            i++;
            continue;
        }
        LexerNode* node = (LexerNode*) Alloc(sizeof(LexerNode));
        node->next= NULL;
        if(isalpha(line[i])){
            int capacity = 10;
            int j = 0;
            node->data.stringData = (char*) Alloc(capacity);
            while(i < n && (isalpha(line[i]) || line[i] == '_')){
                if(j+1 >= capacity){
                    capacity *= 2;
                    node->data.stringData = Realloc(node->data.stringData,capacity);
                }
                node->data.stringData[j++] = line[i++];
            }
            node->data.stringData[j] = '\0';
            if(strcmp(node->data.stringData,"print") == 0){
                node->type  =PRINT;
            }else if(strcmp(node->data.stringData,"input") == 0){
                node->type = INPUT;
            }else if(strcmp(node->data.stringData,"and") == 0){
                node->type = AND;
            }else if(strcmp(node->data.stringData,"or") == 0){
                node->type = OR;
            }else if(strcmp(node->data.stringData,"not") == 0){
                node->type = NOT;
            }else if(strcmp(node->data.stringData,"if") == 0){
                node->type = IF;
            }else if(strcmp(node->data.stringData,"elif") == 0){
                node->type = ELIF;
            }else if(strcmp(node->data.stringData,"else") == 0){
                node->type = ELSE;
            }else if(strcmp(node->data.stringData,"while") == 0){
                node->type = WHILE;
            }else if(strcmp(node->data.stringData,"int") == 0){
                node->type = INT;
            }
            else if(strcmp(node->data.stringData,"None") == 0){
                node->type = NONE;
            }
            else if(strcmp(node->data.stringData,"True") == 0){
                node->type = BOOLEAN;
                node->data.boolData = true;
            }
            else if(strcmp(node->data.stringData,"False") == 0){
                node->type = BOOLEAN;
                node->data.boolData = false;
            }
            else if(strcmp(node->data.stringData,"const") == 0){
                node->type = CONST;
            }
            else{
                node->type = IDENTIFIER;
            }
        }
        else if(isdigit(line[i])){
            double num = 0;
            double fract = 0.1;
            while(i < n && isdigit(line[i])){
                num = num*10 + (line[i++] - '0');
            }
            if(line[i] == '.'){
                i++; 
                if(!isdigit(line[i])) error("Python : No digits after decimal point ! \n");
                while(i < n && isdigit(line[i])){
                    num += fract * (line[i++] - '0');
                    fract /= 10;
                }
            }
            node->type = NUMBER;
            node->data.numData = num;
        }
        else if(line[i] == '"' || line[i] == '\''){
            char quote = line[i++];
            int capacity = 100;
            int j  =0;
            node->data.stringData  = (char*) Alloc(capacity);
            while(i < n && line[i] != quote){
                if(j+1 >= capacity){
                    capacity *= 2;
                    node->data.stringData =Realloc(node->data.stringData,capacity);
                }
                node->data.stringData[j++] = line[i++];
            }
            if(i >= n) error("Python : Unterminated String Literal \n");
            node->data.stringData[j] = '\0';
            node->type = STRING;
            i++;
        }
        else if(line[i] == ':'){
            node->type = COLON;
            i++;
        }
        else if(line[i] == '('){
            node->type = L_PAREN;
            i++;
        }
        else if(line[i] == ')'){
            node->type = R_PAREN;
            i++;
        }
        else if(line[i] == '\n'){
            node->type = NEWLINE;
            i++;
        }
        else if(line[i] == ','){
            node->type = COMMA;
            i++;
        }else if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%' || line[i] == '^'){
            node->type = OPERATOR;
            node->data.charData=line[i];
            i++;
        }else if(line[i] == '!'){
            i++;
            if(line[i] != '='){
                error("Python : Did you mean '!=' ? \n");
            }
            else{
                node->type = NEQ;
                i++;
            }
        }
        else if(line[i] == '='){
            i++;
            if(line[i] == '='){
                node->type = EQ;
                i++;
            }else{
                node->type = EQUALS;
            }
        }
        else if(line[i] == '>'){
            i++;
            if(line[i] == '='){
                node->type = GEQ;
                i++;
            }else{
                node->type = GT;
            }
        }
        else if(line[i] == '<'){
            i++;
            if(line[i] == '='){
                node->type = LEQ;
                i++;
            }else{
                node->type = LT;
            }
        }else{
            error("Python : Undefined Token Found \n");
        }
        pushLexerNode(&head,&tail,node);
        }
        if(!tail || tail->type != NEWLINE){
            LexerNode* newLineNode = (LexerNode*) Alloc(sizeof(LexerNode));
            newLineNode->type = NEWLINE;
            newLineNode->next = NULL;
            pushLexerNode(&head,&tail,newLineNode);
        }
    return head;
    }

LexerNode* LexerHead = NULL;
LexerNode* LexerTail = NULL;
void pushIntoLexerOutput(LexerNode* node){
        if (node == NULL) return;
        if(LexerHead == NULL){
            LexerHead = node;
            LexerTail = node;
        }else{
            LexerTail->next = node;
        }
        while(node && node->next != NULL){
            node = node->next;
        }
        LexerTail = node;
}
bool shouldSkipLine(char* line){
    int n = strlen(line);
    int i =0;
    while(isspace(line[i])) i++;
    if(line[i] == '#' || line[i] == '\n' || line[i] == '\0'){
        return true;
    }
    return false;
}
bool checkInt(char* line){
    int i =0;
    while(isspace(line[i])) i++;
    line += i;
    char* end = line + strlen(line);
    while(end > line && isspace(*(end - 1))){
        *(--end) = '\0';
    }
    if(line[i] == '+' || line[i] == '-') i++;
    if (!isdigit((unsigned char)line[i])) return false;
    for(;line[i];i++){
        if(!isdigit((unsigned char)line[i])) return false;
    }
   
    return true;
}
unsigned int hash(char* line,unsigned int arrSize){
    int n = strlen(line);
    int index = 0;
    for(int i =0;i < n;i++){
        index += line[i];
    }
    return (index*31) % arrSize;
}
typedef enum {
    V_NUMBER,
    V_STRING,
    V_BOOLEAN,
    V_NONE
} DataType;
typedef struct Variable{
    DataType type;
    char* varName;
    bool isConstant;
    union{
        double numData;
        char* stringData;
        bool boolData;
    } data;
    struct Variable* next;
} Variable;

Variable* store[1000];
void set(Variable* v){
    unsigned int index = hash(v->varName,1000);
    if(store[index]){
        Variable* current = store[index];
        bool isUpdate = false;
        while(current != NULL){
            if(strcmp(current->varName,v->varName) == 0){
                if(current->isConstant){
                    printf("Python : Trying to update a constant variable \"%s\". Try without 'const' maybe ? \n",v->varName);
                    freeAlloc();
                    exit(1);
                }
                current->type = v->type;
                if(current->type == V_STRING){
                   char* temp = (char*) Realloc(current->data.stringData,strlen(v->data.stringData)+1 );
                   if(temp == NULL){
                        error("Memory Allocation Failed in Variable Assignment \n");
                   }
                   current->data.stringData = temp;
                   strcpy(current->data.stringData,v->data.stringData);
                }else{
                    current->data  = v->data;
                } 
                isUpdate = true;
                break;
            }
            current = current->next;
        }
        if(!isUpdate) {
            while(current && current->next !=  NULL) current= current->next;
            current->next = v;
        }
    }else{
        store[index] = v;
    }
}
Variable* get(char* varName){
    int index = hash(varName,1000);
    Variable* current = store[index];
    while(current != NULL){
        if(strcmp(varName,current->varName) == 0){
            return current;
        }
        current = current->next;
    }
    return current;
}

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
    AST_IDENTIFIER,
    AST_EQ,
    AST_NEQ,
    AST_LT,
    AST_GT,
    AST_LEQ,
    AST_GEQ,
    AST_ASSIGNMENT
} astNodeType;

typedef struct astNode{
    astNodeType type;
    union{
        double numData;
        char* stringData;
        bool boolData;
        char charData;
    }data;
    struct astNode* nextBlock;
    struct astNode* thenBlock;
    struct astNode* thenNext;
    struct astNode* left;
    struct astNode* right;
    struct astNode* child;
    bool isConstant;
} astNode;

astNode* initNode(){
    astNode* node = (astNode*) Alloc(sizeof(astNode));
    memset(node, 0, sizeof(astNode));
    return node;
}
LexerNode* current = NULL;
void consume(){
    if(current != NULL){
        current = current->next;
    }
}

astNode* parseBlock();
astNode* parseIf();
astNode* parseWhile();
astNode* parseVarDec();
astNode* parseIO();
astNode* parseInt();
astNode* parseExpression();
astNode* parseNot();
astNode* parseAndOr();
astNode* parseComparator();
astNode* parseAddSub();
astNode* parseMulDivMod();
astNode* parseExponent();
astNode* parseAtom();

astNode* parseAtom(){
    if(current->type == NUMBER){
        astNode* node = initNode();
        node->type = AST_NUMBER;
        node->data.numData = current->data.numData;
        consume();
        return node;
    }else if(current->type == STRING){
        astNode* node = initNode();
        node->type = AST_STRING;
        node->data.stringData = (char*) Alloc(strlen(current->data.stringData)+1);
        strcpy(node->data.stringData,current->data.stringData);
        consume();
        return node;
    }else if(current->type == IDENTIFIER){
        astNode* node = initNode();
        node->type = AST_IDENTIFIER;
        node->data.stringData = (char*) Alloc(strlen(current->data.stringData)+1);
        strcpy(node->data.stringData,current->data.stringData);
        consume();
        return node;
    }else if(current->type == BOOLEAN){
        astNode* node = initNode();
        node->type = AST_BOOLEAN;
        node->data.boolData = current->data.boolData;
        consume();
        return node;
    }else if(current->type == NONE){
        astNode* node = initNode();
        node->type = AST_NONE;
        consume();
        return node;
    }else if(current->type == L_PAREN){
        consume();
        astNode* node = parseAddSub();
        if(current == NULL || current->type != R_PAREN){
            error("Missing ) in the expression");
        }
        consume();
        return node;
    } else if(current->type == INT){
        astNode* node = parseInt();
        return node;
    }else if(current->type == INPUT){
        astNode* node = parseIO();
    }
    else{
        error("Undefined Token found while Parsing \n");
    }
}
astNode* parseExponent(){
    astNode* lhs = parseAtom();
    if(lhs == NULL) error("Unexpected Error While Parsing \n");
    while(current != NULL && current->type != NEWLINE && current->type != COMMA && current->type != COLON && (current->data.charData == '^')){
        astNode* parent = initNode();
        parent->type = AST_OPERATOR;
        parent->data.charData = '^';
        consume();
        astNode* rhs = parseAtom();
        if(rhs == NULL) error("Unexpected Error While Parsing \n");
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}
astNode* parseAddSub(){
    astNode* lhs = parseMulDivMod();
    if(lhs == NULL) error("Unexpected Error While Parsing \n");
    while(current != NULL && current->type != NEWLINE && current->type != COMMA && current->type != COLON && (current->data.charData == '+' || current->data.charData == '-' )){
       char op = current->data.charData;
       astNode* parent = initNode();
       parent->type = AST_OPERATOR;
       parent->data.charData = op;
       consume();
       astNode* rhs = parseMulDivMod();
       if(rhs == NULL) error("Unexpected Error While Parsing \n");
       parent->left = lhs;
       parent->right = rhs;
       lhs = parent;
    }
    return lhs;
}

astNode* parseMulDivMod(){
    astNode* lhs = parseExponent();
    if(lhs == NULL) error("Unexpected Error While Parsing \n");
    while(current != NULL && current->type != NEWLINE && current->type != COMMA && current->type != COLON && (current->data.charData == '*' || current->data.charData == '/' || current->data.charData == '%')){
       char op = current->data.charData;
       astNode* parent = initNode();
       parent->type = AST_OPERATOR;
       parent->data.charData = op;
       consume();
       astNode* rhs = parseExponent();
       if(rhs == NULL) error("Unexpected Error While Parsing \n");
       parent->left = lhs;
       parent->right = rhs;
       lhs = parent;
    }
    return lhs;
}

astNode* parseComparator(){
    astNode* lhs = parseAddSub();
    if(lhs == NULL) error("Unexpected Error While Parsing \n");
    while(current != NULL && current->type != NEWLINE && current->type != COMMA && current->type != COLON && (current->type == NEQ || current->type == EQ || current->type == GT || current->type == LT || current->type == LEQ || current->type == GEQ)){
        astNode* parent = initNode();
        astNodeType parentType;
        if(current->type == EQ) parentType = AST_EQ;
        else if(current->type == NEQ) parentType = AST_NEQ;
        else if(current->type == LEQ) parentType = AST_LEQ;
        else if(current->type == GEQ) parentType = AST_GEQ;
        else if(current->type == LT) parentType = AST_LT;
        else if(current->type == GT) parentType = AST_GT;
        parent->type = parentType;
        consume();
        astNode* rhs = parseAddSub();
        if(rhs == NULL) error("Unexpected Error While Parsing \n");
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}

astNode* parseAndOr(){
    astNode* lhs = parseComparator();
    if(lhs == NULL) error("Unexpected Error While Parsing \n");
    while(current != NULL && current->type != NEWLINE && current->type != COMMA && current->type != COLON && (current->type == AND || current->type == OR )){
       astNode* parent = initNode();
       parent->type = current->type == AND ? AST_AND : AST_OR;
       consume();
       astNode* rhs = parseComparator();
       if(rhs == NULL) error("Unexpected Error While Parsing \n");
       parent->left = lhs;
       parent->right = rhs;
       lhs = parent;
    }
    return lhs;
}
astNode* parseNot(){
    if(current->type == NOT){
        consume();
        astNode* node = initNode();
        node->type = AST_NOT;
        node->child =parseAndOr();
        return node;
    }else{
        return parseAndOr();
    }
}
astNode* parseExpression(){
    return parseNot();
}
astNode* parseInt(){
    if(current->type == INT){
        consume();
        if(current->type == L_PAREN){
            consume();
            astNode* node = initNode();
            node->type = AST_INT;
            node->child = parseExpression();
            if(current->type == R_PAREN) {
                consume();
            }
            else {error("Missing ) in int function \n");
            }
            while(current && current->type == NEWLINE) consume();
            return node;
        }else{
            error("Missing ( after int function \n");
        }
    }else{
        return parseExpression();
    }
}
astNode* parseIO(){
    while (current != NULL && (current->type == R_PAREN || current->type == NEWLINE)) consume();
    if (current == NULL) error("Unexpected end of tokens while parsing\n");
    if(current->type == INPUT){
        consume();
        if(current->type == L_PAREN){
            consume();
            astNode* argument = parseExpression();
            astNode* node = initNode();
            node->type = AST_INPUT;
            node->child = argument;
            if(current->type ==R_PAREN){
                consume();
            }else{
                error("Missing ( after input function \n");
            }
            return node;
        }else{
            error("Missing ( after input function \n");
        }
    }else if(current->type == PRINT){
        consume();
        if(current->type == L_PAREN){
            consume();
            astNode* node = initNode();
            node->type = AST_PRINT;
            astNode* tail = NULL;
            while(current != NULL && current->type != R_PAREN){
                astNode* arg = parseExpression();
                if(node->thenBlock == NULL){
                    node->thenBlock = arg;
                    tail = arg;
                }else{
                    tail->nextBlock =arg;
                    tail = arg;
                }
                if(current->type == COMMA){
                    consume();
                }
            }
            if(current->type != R_PAREN){
                error("Missing ) in print \n");
            }
            if(current->type == R_PAREN) consume();
            if(current->type == NEWLINE) consume();
            return node;
        }else{
            error("Missing ) after print function \n");
        }
    }else{
        return NULL;
    }
}
astNode* parseIf(){
    while(current && (current->type == NEWLINE || current->type == INDENT)) consume();
    if(current == NULL) error("Unexpected Error While Parsing");
    if(current->type == IF || current->type == ELIF){
        consume();
        astNode* condition = (astNode*) parseExpression();
        if (condition == NULL) {
            error("Parser bug: condition expression is empty in if statement\n");
        }
        if(current && current->type == COLON){
            consume();
            while(current && current->type == NEWLINE) consume();
            if(current && current->type == INDENT){
                consume();
                astNode* ifNode = (astNode*) initNode();
                ifNode->type = AST_IF;
                ifNode->child = condition;
                ifNode->thenBlock = NULL;
                ifNode->thenNext = NULL;
                ifNode->nextBlock = NULL;
                astNode* tailStatement = NULL;
                astNode* tailBlock = NULL;
                while(current && current->type != DEDENT){
                    astNode* statement = parseBlock();
                    if(ifNode->thenBlock == NULL){
                        ifNode->thenBlock = statement;
                        tailStatement = statement;
                    }else{
                        tailStatement->thenNext = statement;
                        tailStatement = statement;
                    }
                    if (ifNode->thenBlock == NULL) {
                        error("Indentation Error: empty or malformed if block\n");
                    }
                    while(current && current->type == NEWLINE) consume();
                    
                }
                if(current && current->type == DEDENT) consume();
                if(current && current->type == ELIF){
                    astNode* elifNode = (astNode*) parseIf();
                    ifNode->nextBlock = elifNode;
                }else if(current && current->type == ELSE){
                    astNode* elseNode = (astNode*) initNode();
                    elseNode->child = NULL;
                    elseNode->type = AST_ELSE;
                    elseNode->thenBlock = NULL;
                    elseNode->nextBlock = NULL;
                    elseNode->thenNext = NULL;
                    astNode* tailStatementElse = NULL;
                    consume();
                    if(current->type == COLON){
                        consume();
                        while(current && current->type == NEWLINE) consume();
                        if(current && current->type == INDENT){
                            consume();
                            while(current && current->type != DEDENT){
                                astNode* statement = parseBlock();
                                if(elseNode->thenBlock == NULL){
                                    elseNode->thenBlock = statement;
                                    tailStatementElse = statement;
                                }else{
                                    tailStatementElse->thenNext = statement;
                                    tailStatementElse = statement;
                                }
                            }
                            if(current && current->type == DEDENT) consume();
                            while(current && current->type == NEWLINE) consume();
                            ifNode->nextBlock = elseNode;
                        } else{
                            error("Indentation Error \n");
                        }
                    }else{
                        error("Missing ':' after else statement \n");
                    }

                }
                while(current && current->type == NEWLINE) consume();
                return ifNode;
            }else{
                error("Indentation Error \n");
            }
        }else{
            error("Missing ':' in conditional statment \n");
        }
        
    }else{
        return NULL;
    }
}
astNode* parseWhile(){
    while (current != NULL && current->type == NEWLINE) consume();
    if (current == NULL) error("Unexpected end of tokens while parsing\n");
    if(current->type == WHILE){
        consume();
        astNode* condition = parseExpression();
        if(current->type == COLON){
            consume();
            while (current != NULL && current->type == NEWLINE) consume();
            if(current->type == INDENT){
                consume();
                astNode* whileNode = initNode();
                whileNode->type = AST_WHILE;
                whileNode->thenBlock = NULL;
                whileNode->thenNext = NULL;
                whileNode->child = condition;
                astNode* tail = NULL;
                while(current!= NULL && current->type != DEDENT){
                    astNode* thenBlock = parseBlock();
                    if(whileNode->thenBlock == NULL){
                        whileNode->thenBlock = thenBlock;
                        tail = thenBlock;
                    }else{
                        tail->thenNext = thenBlock;
                        tail = thenBlock;
                    }
                }
                if(current->type == DEDENT) consume();
                while (current != NULL && current->type == NEWLINE) consume();
                return whileNode;
            } else{
                error("Indentation Error \n");
            }
        }else{
            error("Missing ':' in while loop \n");
        }
    }else{
        return NULL;
    }
}
astNode* parseVarDec(){
    while (current != NULL && current->type == NEWLINE) consume();
    if (current == NULL) error("Unexpected end of tokens while parsing\n");
    if(current->type == IDENTIFIER || current->type == CONST){
        astNode* node = initNode();
        node->type = AST_ASSIGNMENT;
        if(current->type == CONST) {
            node->isConstant = true;
            consume();}
        else{
            node->isConstant = false;
        }
        node->data.stringData = Alloc(strlen(current->data.stringData)+1);
        strcpy(node->data.stringData,current->data.stringData);
        consume();
        if(current->type == EQUALS){
            consume();
            astNode* value = parseExpression();
            node->child = value;
            while (current && current->type == NEWLINE) consume();
            return node;
        }else{
            error("Missing = in variable assignment statement \n");
        }

    }else{
        while (current && current->type == NEWLINE) consume();
        return NULL;
    }
}
astNode* parseBlock(){
    while(current && (current->type == NEWLINE ||  current->type == INDENT)) consume();
    if (current == NULL) error("Undefined Error While Parsing A Block\n");
    astNode* node = NULL;
    if(current->type == IDENTIFIER || current->type == CONST){
        node =  parseVarDec();
    }else if(current->type == PRINT || current->type == INPUT){
        node =  parseIO();
    }else if(current->type == IF){
        node = parseIf();
    }else if(current->type == WHILE){
        node=  parseWhile();
    }
    else if (current->type == ELSE) {
        return NULL;
    }
    else if(current == NULL || current->next == NULL){
        return NULL;
    }
    else{
        error("Undefined Error While Parsing A Block\n");
    }
    while(current && current->type == NEWLINE) consume();
    return node;
}

typedef struct{
    DataType type;
    union{
        double numData;
        char* stringData;
        bool boolData;
    }data;
} Value;

Value* createValue() {
    Value* v = (Value*)malloc(sizeof(Value));
    v->type = V_NONE;
    v->data.numData = 0;
    v->data.boolData = false;
    v->data.stringData = NULL;
    return v;
}
Value* evaluate(astNode* node){
    if (node == NULL) {
    printf("EVAL-BUG: evaluate() called with NULL node\n");
    error("Internal parser bug: NULL AST node\n");
}
    if(node->type == AST_NUMBER){
        Value* v = createValue();
        v->type = V_NUMBER;
        v->data.numData = node->data.numData;
        return v;
    }else if(node->type == AST_STRING){
        Value* v = createValue();
        v->type = V_STRING;
        v->data.stringData = Alloc(strlen(node->data.stringData) + 1);
        strcpy(v->data.stringData,node->data.stringData);
        return v;
    }else if(node->type == AST_BOOLEAN){
        Value* v = createValue();
        v->type = V_BOOLEAN;
        v->data.boolData = node->data.boolData;
        return v;
    }else if(node->type == AST_NONE){
        Value* v = createValue();
        v->type = V_NONE;
        return v;
    }
    else if(node->type == AST_IDENTIFIER){
        Variable* var = get(node->data.stringData);
        if(var == NULL){
            error("Usaage of Undefined Variable\n");
        }
        Value* v = createValue();
        switch(var->type){
            case V_NUMBER:
                v->type = V_NUMBER;
                v->data.numData = var->data.numData;
                break;
            case V_STRING:
                v->type = V_STRING;
                v->data.stringData = Alloc(strlen(var->data.stringData) + 1);
                strcpy(v->data.stringData,var->data.stringData);
                break;
            case V_BOOLEAN:
                v->type = V_BOOLEAN;
                v->data.boolData =var->data.boolData;
                break;
            case V_NONE:
                v->type = V_NONE;
                break;
            default:
                break;
            }
            return v;
        }
    else if(node->type == AST_OPERATOR){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(node->data.charData == '^'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = pow(lhs->data.numData,rhs->data.numData);
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else{
                error("Using ^ where it should'nt be used. Check Types! \n");
            }
        }
        else if(node->data.charData == '+'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = lhs->data.numData + rhs->data.numData;   
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }
            else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
                Value* val = createValue();
                val->type = V_NUMBER;
                if(lhs->type == V_NUMBER) {
                    val->data.numData = lhs->data.numData + rhs->data.boolData;
                }
                else{
                     val->data.numData = lhs->data.boolData + rhs->data.numData;
                }
                 if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if((lhs->type == V_STRING && rhs->type == V_STRING)){
                Value* val = createValue();
                val->type = V_STRING;
                val->data.stringData = (char*) Alloc(strlen(lhs->data.stringData) + strlen(rhs->data.stringData) + 1);
                strcpy(val->data.stringData,lhs->data.stringData);
                strcat(val->data.stringData,rhs->data.stringData);
                return val;
            }
            else if(lhs->type == V_BOOLEAN &&  rhs->type == V_BOOLEAN){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = lhs->data.boolData + rhs->data.boolData;
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }
            else{
                error("Using '+' operator where it shouldn't be used ! \n");
            }
        }
        else if(node->data.charData == '-'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = lhs->data.numData - rhs->data.numData;
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
                Value* val = createValue();
                val->type = V_NUMBER;
                if(lhs->type == V_NUMBER){
                    val->data.numData = lhs->data.numData - rhs->data.boolData;
                }else{
                    val->data.numData = lhs->data.boolData - rhs->data.numData;
                }
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else{
                error("Using '-' operator where it shouldn't be used ! \n");
            }
        }else if(node->data.charData == '*'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = lhs->data.numData * rhs->data.numData;
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
                Value* val = createValue();
                val->type = V_NUMBER;
                if(lhs->type == V_NUMBER){
                    val->data.numData = lhs->data.numData * rhs->data.boolData;
                }else{
                    val->data.numData = lhs->data.boolData * rhs->data.numData;
                }
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = lhs->data.boolData * rhs->data.boolData;
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if((lhs->type == V_STRING && rhs->type == V_NUMBER) || (lhs->type == V_NUMBER && rhs->type == V_STRING)){
                Value* val = createValue();
                val->type = V_STRING;
                if(lhs->type == V_STRING){
                    if(rhs->data.numData < 0){
                        error("Negative product of string is invalid \n");
                    }
                    val->data.stringData = (char*) Alloc(strlen(lhs->data.stringData) * rhs->data.numData + 1);
                    int k = 0;
                    while(k < rhs->data.numData){
                        if(k == 0){
                            strcpy(val->data.stringData,lhs->data.stringData);
                        }else{
                            strcat(val->data.stringData,lhs->data.stringData);
                        }
                        k++;
                    }
                }else{
                    if(lhs->data.numData < 0){
                        error("Negative product of string is invalid \n");
                    }
                    val->data.stringData = (char*) Alloc(strlen(rhs->data.stringData) * lhs->data.numData + 1);
                    int k = 0;
                    while(k < lhs->data.numData){
                        if(k == 0){
                            strcpy(val->data.stringData,rhs->data.stringData);
                        }else{
                            strcat(val->data.stringData,rhs->data.stringData);
                        }
                        k++;
                    }
                }
                return val;
            }
            else{
               error("Using '*' operator where it shouldn't be used ! \n");
            }
        }
        else if(node->data.charData == '/'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                if(rhs->data.numData == 0){
                    error("Division By Zero \n");
                }
                val->data.numData = lhs->data.numData / rhs->data.numData;
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
                Value* val = createValue();
                val->type = V_NUMBER;
                if(lhs->type == V_NUMBER){
                    if(rhs->data.boolData == false){
                        error("Division By Zero \n");
                    }
                    val->data.numData = lhs->data.numData / rhs->data.boolData;
                }else{
                    if(rhs->data.numData == 0){
                        error("Division By Zero \n");
                    }
                    val->data.numData = lhs->data.boolData / rhs->data.numData;
                }
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }

                return val;
            }else{
                error("Using '/' where it should'nt be used \n");
            }
        }else if(node->data.charData == '%'){
            if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData = fmod(lhs->data.numData,rhs->data.numData);
                if(val->data.numData == floor(val->data.numData)){
                    val->data.numData = floor(val->data.numData);
                }
                return val;
            }else{
                error("Using '%' where it should'nt be used \n");
            }
        }
    }
    else if(node->type == AST_AND){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData && rhs->data.boolData;
            return val;
        }else if((lhs->type == V_BOOLEAN && rhs->type == V_NUMBER) || (lhs->type == V_NUMBER && rhs->type == V_BOOLEAN)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_BOOLEAN){
                val->data.boolData = lhs->data.boolData && (rhs->data.numData == 0 ? false : true);
            }else{
                val->data.boolData = rhs->data.boolData && (lhs->data.numData == 0 ? false : true);
            }
            return val;
        }else if((lhs->type == V_BOOLEAN && rhs->type == V_STRING) || (lhs->type == V_STRING && rhs->type ==V_BOOLEAN)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_BOOLEAN){
                val->data.boolData = lhs->data.boolData && (strlen(rhs->data.stringData) != 0);
            }else{
                val->data.boolData = rhs->data.boolData &&  (strcmp(lhs->data.stringData,"") != 0);
            }
            return val;
        }else if(lhs->type == V_NONE || rhs->type == V_NONE){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = false;
            return val;
        }
        else if((lhs->type == V_NUMBER && rhs->type == V_STRING) || (lhs->type == V_STRING && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData && (strlen(rhs->data.stringData) != 0);
            }else{
                val->data.boolData = rhs->data.numData &&  (strlen(lhs->data.stringData) != 0);
            }
            return val;
        }
        else{
            error("and operation is used incorrectly \n");
        }
    }else if(node->type == AST_OR){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData || rhs->data.boolData;
            return val;
        }else if((lhs->type == V_BOOLEAN && rhs->type == V_NUMBER) || (lhs->type == V_NUMBER && rhs->type == V_BOOLEAN)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_BOOLEAN){
                val->data.boolData = lhs->data.boolData || (rhs->data.numData == 0 ? false : true);
            }else{
                val->data.boolData = rhs->data.boolData || (lhs->data.numData == 0 ? false : true);
            }
            return val;
        }else if((lhs->type == V_BOOLEAN && rhs->type == V_STRING) || (lhs->type == V_STRING && rhs->type ==V_BOOLEAN)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_BOOLEAN){
                val->data.boolData = lhs->data.boolData || (strlen(rhs->data.stringData) != 0);
            }else{
                val->data.boolData = rhs->data.boolData || (strlen(lhs->data.stringData) != 0);
            }
            return val;
        }else if((lhs->type == V_NONE && rhs->type ==  V_NUMBER) || (lhs->type == V_NUMBER && rhs->type == V_NONE)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NONE){
                if(rhs->data.numData != 0){
                    val->data.boolData = true;
                }else{
                    val->data.boolData = false;
                }
            }else{
                 if(lhs->data.numData != 0){
                    val->data.boolData = true;
                }else{
                    val->data.boolData = false;
                }
            }
            return val;
        }
        else if((lhs->type == V_NONE && rhs->type ==  V_STRING) || (lhs->type == V_STRING && rhs->type == V_NONE)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NONE){
                if(strlen(rhs->data.stringData) != 0){
                    val->data.boolData = true;
                }else{
                    val->data.boolData = false;
                }
            }else{
                 if(strlen(lhs->data.stringData) != 0){
                    val->data.boolData = true;
                }else{
                    val->data.boolData = false;
                }
            }
        }
        else if((lhs->type == V_NUMBER && rhs->type == V_STRING) || (lhs->type == V_STRING && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData && (strlen(rhs->data.stringData) != 0);
            }else{
                val->data.boolData = rhs->data.numData &&  (strlen(lhs->data.stringData) != 0);
            }
            return val;
        }
        else{
            error("and operation is used incorrectly \n");
        }
    }
    else if(node->type == AST_NOT){
        Value* result= evaluate(node->child);
        if(result == NULL) error("Unexpected Error while parsing \n");
        Value* val = createValue();
        val->type = V_BOOLEAN;
        if(result->type == V_BOOLEAN){
            val->data.boolData = !(result->data.boolData);
        }else if(result->type == V_NUMBER){
            if(result->data.numData != 0) val->data.boolData = true;
            else val->data.boolData = false;
        }else if(result->type == V_STRING){
            if(strlen(result->data.stringData) == 0) val->data.boolData = true;
            else val->data.boolData = false;
        }else if(result->type == V_NONE){
            val->data.boolData = false;
        }
        return val;
    }
    else if(node->type == AST_GT){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.numData > rhs->data.numData;
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData > rhs->data.boolData;
            }else{
                val->data.boolData = lhs->data.boolData > rhs->data.numData;
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData > rhs->data.boolData;
            return val;
        }else{
            error("Incorrect Usage of > in statment. Check Types \n");
        }
    }else if(node->type == AST_LT){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.numData < rhs->data.numData;
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData < rhs->data.boolData;
            }else{
                val->data.boolData = lhs->data.boolData < rhs->data.numData;
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData < rhs->data.boolData;
            return val;
        }else{
            error("Incorrect Usage of < in statment. Check Types \n");
        }
    }else if(node->type == AST_GEQ){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.numData >= rhs->data.numData;
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData >=  rhs->data.boolData;
            }else{
                val->data.boolData = lhs->data.boolData >=  rhs->data.numData;
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData >=  rhs->data.boolData;
            return val;
        }else{
            error("Incorrect Usage of >= in statment. Check Types \n");
        }
    }else if(node->type == AST_LEQ){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.numData <= rhs->data.numData;
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData <=  rhs->data.boolData;
            }else{
                val->data.boolData = lhs->data.boolData <=  rhs->data.numData;
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_BOOLEAN;
            val->data.boolData = lhs->data.boolData <=  rhs->data.boolData;
            return val;
        }else{
            error("Incorrect Usage of <= in statment. Check Types \n");
        }
    }else if(node->type == AST_EQ){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        Value* val = createValue();
        val->type = V_BOOLEAN;
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            val->data.boolData = (lhs->data.numData == rhs->data.numData);
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            if(lhs->type == V_NUMBER){
                val->data.boolData = lhs->data.numData ==  rhs->data.boolData;
            }else{
                val->data.boolData = lhs->data.boolData ==  rhs->data.numData;
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            val->data.boolData = (lhs->data.boolData == rhs->data.boolData);
            return val;
        }
        else if(lhs->type == V_STRING && rhs->type == V_STRING){
            if(lhs->data.stringData && rhs->data.stringData){
               val->type = V_BOOLEAN;
                if(strcmp(lhs->data.stringData,rhs->data.stringData) == 0){
                    val->data.boolData = true;
                }else{
                    val->data.boolData = false;
                }
                return val;
            }else{
                 error("Undefined String Value (String maybe incomplete or misused) \n");
            }
            
        }else if(lhs->type == V_NONE || rhs->type == V_NONE){
            if(lhs->type  != rhs->type){
                val->data.boolData = false;
            }else{
                val->data.boolData = true;
            }
            return val;
        }
        else{
            error("Incorrect Usage of == in statment. Check Types \n");
        }
    }
    else if(node->type == AST_NEQ){
        Value* lhs = evaluate(node->left);
        Value* rhs = evaluate(node->right);
        Value* val = createValue();
        val->type = V_BOOLEAN;
        if(lhs->type == V_NUMBER && rhs->type == V_NUMBER){
            val->data.boolData = !(lhs->data.numData == rhs->data.numData);
            return val;
        }else if((lhs->type == V_NUMBER && rhs->type == V_BOOLEAN) || (lhs->type == V_BOOLEAN && rhs->type == V_NUMBER)){
            if(lhs->type == V_NUMBER){
                val->data.boolData = !(lhs->data.numData ==  rhs->data.boolData);
            }else{
                val->data.boolData = !(lhs->data.boolData ==  rhs->data.numData);
            }
            return val;
        } else if(lhs->type == V_BOOLEAN && rhs->type == V_BOOLEAN){
            val->data.boolData = !(lhs->data.boolData == rhs->data.boolData);
            return val;
        }
        else if(lhs->type == V_STRING && rhs->type == V_STRING){
            if(lhs->data.stringData && rhs->data.stringData){
               val->type = V_BOOLEAN;
                if(strcmp(lhs->data.stringData,rhs->data.stringData) == 0){
                    val->data.boolData = false;
                }else{
                    val->data.boolData = true;
                }
                return val;
            }else{
                 error("Undefined String Value (String maybe incomplete or misused) \n");
            }
            
        }else if(lhs->type == V_NONE || rhs->type == V_NONE){
            if(lhs->type  != rhs->type){
                val->data.boolData = true;
            }else{
                val->data.boolData = false;
            }
            return val;
        }
        else{
            error("Incorrect Usage of != in statment. Check Types \n");
        }
    }
    else if(node->type == AST_INT){
        Value* child = evaluate(node->child);
        if(child->type == V_BOOLEAN){
            Value* val = createValue();
            val->type = V_NUMBER;
            if(child->data.boolData == true){
                val->data.numData = 1;
            }else{
                val->data.numData = 0;
            }
            return val;
        }else if(child->type == V_STRING){
            char* numString = child->data.stringData;
            bool checkValid = checkInt(numString);
            if(!checkValid){
                error("Invalid argument for int function ,check for spaces or alphabets and remove them for int conversion\n");
            }else{
                int num = atoi(numString);
                Value* val = createValue();
                val->type = V_NUMBER;
                val->data.numData= num;
                return val;
            }
        }
        else if(child->type == V_NUMBER){
            Value* val = createValue();
            val->type = V_NUMBER;
            val->data.numData = floor(child->data.numData);
            return val;
        }
        else{
            error("Invalid argument for int function \n");
        }
    }else if(node->type == AST_INPUT){
        Value* child = evaluate(node->child);
        char inputArg[100];
        if(child->type == V_STRING){
            printf("%s",child->data.stringData);
            fgets(inputArg,sizeof(inputArg),stdin);
            inputArg[strcspn(inputArg,"\r\n")] = '\0';
        }else if(child->type == V_NUMBER){
            printf("%d",child->data.numData);
            fgets(inputArg,sizeof(inputArg),stdin);
            inputArg[strcspn(inputArg,"\r\n")] = '\0';
        }else if(child->type == V_BOOLEAN){
            printf("%s",child->data.boolData ? "True" : "False");
            fgets(inputArg,sizeof(inputArg),stdin);
            inputArg[strcspn(inputArg,"\r\n")] = '\0';
        }else if(child->type == V_NONE){
            printf("%s","None");
            fgets(inputArg,sizeof(inputArg),stdin);
            inputArg[strcspn(inputArg,"\r\n")] = '\0';
        }
        Value* val = createValue();
        val->type = V_STRING;
        val->data.stringData = (char*) Alloc(strlen(inputArg)+1);
        strcpy(val->data.stringData,inputArg);
        return val;
    }else if(node->type == AST_PRINT){
        astNode* arg = node->thenBlock;
        while(arg != NULL){
            Value* v = evaluate(arg);
            switch(v->type){
                case V_NUMBER:
                    if(v->data.numData == floor(v->data.numData)){
                        printf("%0.f",v->data.numData);
                    }else{
                        printf("%g",v->data.numData);
                    }
                    break;
                case V_STRING:
                    printf("%s",v->data.stringData);
                    break;
                case V_BOOLEAN:
                    printf("%s",v->data.boolData ? "True" : "False");
                    break;
                case V_NONE:
                    printf("%s","None");
                    break;
                default:
                    break;
                }
            if(arg->nextBlock != NULL) printf(" ");
            arg = arg->nextBlock;
        }
        printf("\n");
    }else if(node->type == AST_ASSIGNMENT){
        Value* value = evaluate(node->child);
        char* varName = node->data.stringData;
        Variable* v = Alloc(sizeof(Variable));
        v->varName = (char*) Alloc(strlen(varName) + 1);
        if(value->type == V_NUMBER){
            v->type = V_NUMBER;
            v->data.numData = value->data.numData;
        }else if(value->type == V_STRING){
            v->type = V_STRING;
            v->data.stringData = (char*) Alloc(strlen(value->data.stringData) + 1);
            strcpy(v->data.stringData,value->data.stringData);
        }else if(value->type == V_BOOLEAN){
            v->type = V_BOOLEAN;
            v->data.boolData = value->data.boolData;
        }else if(value->type == V_NONE){
            v->type = V_NONE;
        }
        v->isConstant = node->isConstant;
        strcpy(v->varName,varName);
        set(v);
    }else if(node->type == AST_IF){
        astNode* current_then = node->thenBlock;
        Value* condition = evaluate(node->child);
        if(condition->type == V_NUMBER){
            condition->type = V_BOOLEAN;
            condition->data.boolData = condition->data.numData  != 0 ? true : false;
        }else if(condition->type == V_STRING){
            condition->type = V_BOOLEAN;
            condition->data.boolData = strlen(condition->data.stringData) != 0 ? true : false;
        }else if(condition->type == V_NONE){
            condition->data.boolData = false;
        }
        if(condition->data.boolData == true){
            while(current_then != NULL){
                Value* eval = evaluate(current_then);
                current_then = current_then->thenNext;
            }
        }else if(node->nextBlock != NULL ){
            if(node->nextBlock->type == AST_IF){
                Value* eval = evaluate(node->nextBlock);
            }else if(node->nextBlock->type == AST_ELSE){
                astNode* current_then = node->nextBlock->thenBlock;
                while(current_then != NULL){
                    Value* eval = evaluate(current_then);
                    current_then =  current_then->thenNext;
                }
            }
            
        }
    }
    else if(node->type == AST_WHILE){
        Value* condition = evaluate(node->child);
        while(condition && condition->data.boolData == true){
            astNode* body = node->thenBlock;
            while(body != NULL){
                Value* eval = evaluate(body);
                body = body->thenNext;
            }
            condition = evaluate(node->child);
        }
    }else{
        return NULL;
    }
}

void interpret(char* fileName){
    FILE* fptr= fopen(fileName,"r");
    LexerNode* tail = NULL;
    if(fptr == NULL) error("File Not Found For Interpretation \n");
    s = initStack();
    pushStack(s,0);
    char line[100];
    while(fgets(line,100,fptr)){
        if(shouldSkipLine(line)){
            continue;
        }
        LexerNode* line_output = (LexerNode*) Lexer(line);
        pushIntoLexerOutput(line_output);
    }
    while(s->top  != NULL && s->top->value > 0){
        int popped = popStack(s);
        LexerNode* dedentNode = (LexerNode*) Alloc(sizeof(LexerNode));
        dedentNode->type = DEDENT;
        dedentNode->next = NULL;
        pushIntoLexerOutput(dedentNode);
    }
    current = LexerHead;
    while(current != NULL){
        while(current && current->type == NEWLINE) consume();
        if (current == NULL) break;
        astNode* node = parseBlock();
        Value* eval = evaluate(node);
    }

    fclose(fptr);
    freeAlloc();
}
int main(int argCount,char* argv[]){
    if(argCount == 1){
        error("Missing FileName for Interpretation \n");
    }else{
        char* fileName = argv[1];
        char* end = fileName + strlen(fileName);
        if(*(end-1) != 'y' || *(end-2) != 'p' || *(end-3) != '.'){
            error("File Type .py is mandatory for Interpretation \n");
        }else{
            interpret(fileName);
        }
    }
    return 0;
}