#include <memoryHandler.h>
#include <error.h>
#include <lexer.h>
#include <string.h>
#include <ctype.h>
#include <indentation.h>
#include <globals.h>

void Lexer(char* fileName){
    char line[256];
    FILE* fptr = fopen(fileName,"r");
    if(fptr == NULL){
        char err[256];
        snprintf(err,sizeof(err),"The File '%s' Is Not Found",fileName);
        error(err,0,FILE_NOT_FOUND_ERROR);
    }
    int lineCount = 1;
    stk = createStack();
    pushStack(stk,0);
    while(fgets(line,sizeof(line),fptr)){
        line_lexer(line,lineCount,stk);
        lineCount++;
    }
    while(peekStack(stk) > 0){
        popStack(stk);
        MemNode* node = createMemNode(sizeof(LexerNode));
        LexerNode* temp = node->ptr;
        temp->type = DEDENT;
        temp->next = NULL;
        temp->lineCount  = lineCount;
        pushIntoLexerOutput(temp);
    }
    fclose(fptr);
}
void pushIntoLexerOutput(LexerNode* node){
    if(lexer_output == NULL){
        lexer_output = node;
        lexer_tail = node;
    }else{
        lexer_tail->next = node;
        lexer_tail = node;
    }
}
void line_lexer(char* line,int lineCount,Stack* stk){
    int i =0;
    int n = strlen(line);
    bool commentFound = false;
    int current_space = 0;
    int prev_indent = peekStack(stk);
    while(i < n && isspace(line[i])) i++;
    if(line[i] == '\n' || line[i] == '\0' || line[i] == '#'){
        MemNode* node = createMemNode(sizeof(LexerNode));
        LexerNode* newLineNode = node->ptr;
        newLineNode->type = NEWLINE;
        newLineNode->lineCount = lineCount;
        newLineNode->next = NULL;
        pushIntoLexerOutput(newLineNode);
        return;
    }
    i= 0;
    while(line[i] == ' ' || line[i] == '\t') {
        current_space++;
        i++;
    }
    if(stk->top != NULL && prev_indent < current_space){
        pushStack(stk,current_space);
        MemNode* node = createMemNode(sizeof(LexerNode));
        LexerNode* temp = node->ptr;
        temp->next = NULL;
        temp->type = INDENT;
        temp->lineCount = lineCount;
        pushIntoLexerOutput(temp);
    }else if(stk->top != NULL && prev_indent > current_space){
        while(stk->top != NULL && peekStack(stk) > current_space){
            popStack(stk);
            MemNode* node = createMemNode(sizeof(LexerNode));
            LexerNode* temp = node->ptr;
            temp->next = NULL;
            temp->lineCount = lineCount;
            temp->type = DEDENT;
            pushIntoLexerOutput(temp);
        }
        if(current_space != peekStack(stk)){
            error("Indentation Does Not Match",lineCount,INDENTATION_ERROR);
        }
    }else{
        // eat 5 star , do nothing
    }
    while(i < n){
        if(isspace(line[i])){
            i++;
            continue;
        }else{
            MemNode* node = createMemNode(sizeof(LexerNode));
            LexerNode* temp = node->ptr;
            temp->lineCount = lineCount;
            if(isalpha(line[i])){
                int j =0;
                int capacity = 100;
                MemNode* strobj = createMemNode(capacity);
                temp->data.strData = (char*) strobj->ptr;
                while(i < n && (isalnum(line[i]) || line[i] == '_')){
                    if(j >= capacity){
                        capacity *= 2;
                        reallocMemNode(strobj,capacity);
                    }
                    temp->data.strData[j++] = line[i++];
                }
                temp->data.strData[j] = '\0';
                if(strcmp(temp->data.strData,"print") == 0){
                    temp->type  =PRINT;
                }else if(strcmp(temp->data.strData,"input") == 0){
                    temp->type = INPUT;
                }
                else if(strcmp(temp->data.strData,"include") == 0){
                    temp->type = INCLUDE;
                }
                else if(strcmp(temp->data.strData,"and") == 0){
                    temp->type = AND;
                }else if(strcmp(temp->data.strData,"or") == 0){
                    temp->type = OR;
                }else if(strcmp(temp->data.strData,"not") == 0){
                    temp->type = NOT;
                }else if(strcmp(temp->data.strData,"if") == 0){
                    temp->type = IF;
                }else if(strcmp(temp->data.strData,"elif") == 0){
                    temp->type = ELIF;
                }else if(strcmp(temp->data.strData,"else") == 0){
                    temp->type = ELSE;
                }else if(strcmp(temp->data.strData,"while") == 0){
                    temp->type = WHILE;
                }
                else if(strcmp(temp->data.strData,"for") == 0){
                    temp->type = FOR;
                }
                else if(strcmp(temp->data.strData,"to") == 0){
                    temp->type = TO;
                }
                else if(strcmp(temp->data.strData,"in") == 0){
                    temp->type = IN;
                }
                else if(strcmp(temp->data.strData,"int") == 0){
                    temp->type = INT;
                }
                else if(strcmp(temp->data.strData,"str") == 0){
                    temp->type = STR;
                }
                else if(strcmp(temp->data.strData,"len") == 0){
                    temp->type = LEN;
                }
                else if(strcmp(temp->data.strData,"type") == 0){
                    temp->type = TYPE;
                }
                else if(strcmp(temp->data.strData,"bool") == 0){
                    temp->type = BOOL;
                }
                else if(strcmp(temp->data.strData,"None") == 0){
                    temp->type = NONE;
                }
                else if(strcmp(temp->data.strData,"True") == 0){
                    temp->type = BOOLEAN;
                    temp->data.boolData = true;
                }
                else if(strcmp(temp->data.strData,"False") == 0){
                    temp->type = BOOLEAN;
                    temp->data.boolData = false;
                }
                else if(strcmp(temp->data.strData,"const") == 0){
                    temp->type = CONST;
                }else if(strcmp(temp->data.strData,"fn") == 0){
                    temp->type = FUNCTION;
                }else if(strcmp(temp->data.strData,"return") == 0){
                    temp->type = RETURN;
                }
                else if(strcmp(temp->data.strData,"load") == 0){
                    temp->type = LOAD;
                }
                else{
                    temp->type = IDENTIFIER;
                }
                temp->next = NULL;
            }else if(isdigit(line[i])){
                double num = 0.0;
                while(isdigit(line[i])){
                    num = num * 10 + (line[i] - '0');
                    i++;
                }
                if(line[i] == '.'){
                    i++;
                    double fract = 10;
                    while(isdigit(line[i])){
                        num = num + (line[i] - '0') / fract;
                        fract *= 10;
                        i++;
                    }
                }
                temp->data.numData = num;
                temp->type = NUMBER;
                temp->next = NULL;
            }
            else if(line[i] == '\'' || line[i] == '"'){
                char quote = line[i];
                i++;
                int j =0;
                int capacity = 100;
                MemNode* strObj = createMemNode(capacity);
                temp->data.strData = strObj->ptr;
                while(i < n && line[i] != quote){ 
                    if(j >= capacity){
                        capacity *= 2;
                        reallocMemNode(strObj,capacity);
                    }
                    temp->data.strData[j++] = line[i++];
                }
                if(line[i] == quote){
                    i++;
                }else{
                    error("Missing Quotation For String Declaration",lineCount,SYNTAX_ERROR);
                }
                temp->data.strData[j] = '\0';
                temp->type = STRING;
                temp->next = NULL;
            }
            else if(line[i] == ':'){
                temp->type = COLON;
                i++;
            }
            else if(line[i] == '('){
                temp->type = L_BRACK;
                i++;
            }
            else if(line[i] == ')'){
                temp->type = R_BRACK;
                i++;
            }
            else if(line[i] == '['){
                temp->type = L_SQUARE_BRACK;
                i++;
            }
            else if(line[i] == ']'){
                temp->type = R_SQUARE_BRACK;
                i++;
            }
            else if(line[i] == '\n'){
                temp->type = NEWLINE;
                i++;
            }
            else if(line[i] == ','){
                temp->type = COMMA;
                i++;
            }else if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%' || line[i] == '^'){
                temp->type = OPERATOR;
                temp->data.charData=line[i];
                i++;
            }else if(line[i] == '!'){
                i++;
                if(line[i] != '='){
                  error("Python : Did you mean '!=' ? \n",lineCount,SYNTAX_ERROR);
                }
            else{
                    temp->type = NEQ;
                    i++;
                }
            }
            else if(line[i] == '='){
                i++;
                if(line[i] == '='){
                    temp->type = EQ;
                    i++;
                }else{
                    temp->type = EQUALS;
                }
            }
            else if(line[i] == '>'){
                i++;
                if(line[i] == '='){
                    temp->type = GEQ;
                    i++;
                }else{
                    temp->type = GT;
                }
            }
            else if(line[i] == '<'){
                i++;
                if(line[i] == '='){
                    temp->type = LEQ;
                    i++;
                }else{
                    temp->type = LT;
                }
                temp->next = NULL;
            }
            else if(line[i] == '#'){
                temp->type = NEWLINE;
                pushIntoLexerOutput(temp);
                commentFound = true;
                break;
            }
            else{
                error("Undefined Token Found By Lexer \n",lineCount,SYNTAX_ERROR);
            }
            pushIntoLexerOutput(temp);
        }
    }
    if(!commentFound){
        MemNode* newlineNode = createMemNode(sizeof(LexerNode));
        LexerNode* tempNewlineNode = newlineNode->ptr;
        tempNewlineNode->next = NULL;
        tempNewlineNode->type = NEWLINE;
        tempNewlineNode->lineCount = lineCount;
        pushIntoLexerOutput(tempNewlineNode);
    }
}


