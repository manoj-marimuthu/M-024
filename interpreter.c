#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
typedef struct GcNode{
    void* ptr;
    struct GcNode* next;
} GcNode;

GcNode* GC = NULL;
void* GC_create(size_t n){
    void* ptr = malloc(n);
    if(!ptr){
        printf("Python Error : Memory Allocation Failed in Garbage Collector \n");
        exit(1);
    }
    GcNode* temp = malloc(sizeof(GcNode));
    if(!temp){
        printf("Python Error : Memory Allocation Failed in Garbage Collector \n");
        exit(1);
    }
    temp->ptr = ptr;
    temp->next = GC;
    GC = temp;
    return ptr;
}

void freeGC(){
    if(GC == NULL) return;
    GcNode* current = GC;
    while(current != NULL){
        GcNode* next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
}

typedef enum{
    NUMBER,
    OPERATOR,
    STRING,
    IDENTIFIER,
    AND,
    OR,
    NOT,
    BOOLEAN,
    NONE,
    IMPORT,
    CONST,
    PRINTC,
    PRINT,
    L_PARAN,
    R_PARAN,
    EQUALS
} LexerNodeType;
typedef struct LexerNode{
    LexerNodeType type;
    union{
        double numData;
        char charData;
        char *stringData;
        bool boolData;
    } data;
    struct LexerNode* next;
} LexerNode;

LexerNode* LineLexer(char* line){
    int n = strlen(line);
    int i = 0;
    LexerNode* head = NULL,*tail = NULL;
    while(i < n){
        if(isspace(line[i])){
            i++;
            continue;
        }
        if(line[i] == '#'){
            break;
        }
        LexerNode* node = GC_create(sizeof(LexerNode));
        if(line[i] == '\'' || line[i] == '"'){
            char quote = line[i];
            size_t totalLen = 0;
            i++;
            int j = i;
            while(j < n && line[j] != quote){
                totalLen++; 
                j++;
            }
            if(j >= n){
                printf("Python Error : Closing Quote missing \n");
                freeGC();
                exit(1);
            }
            j = 0;
            node->data.stringData = (char *) GC_create(totalLen + 1);
            while(i < n && line[i] != quote){
                node->data.stringData[j++] = line[i++];
            }
            node->data.stringData[j] = '\0';
            node->type = STRING;
            i++;
        }
        else if(isalpha(line[i])){
            int j = 0,k  =i;
            size_t totalLen = 0;
            while(k < n && (isalnum(line[k]) || line[k] == '_')){
                totalLen++;
                k++;
            }
            node->data.stringData = (char*) GC_create(totalLen + 1);
            while(i < n && (isalnum(line[i]) || line[i] == '_')){
                node->data.stringData[j++] = line[i++]; 
            }
            node->data.stringData[j] = '\0';
            if(strcmp(node->data.stringData,"print") == 0){
                node->type = PRINT;
            }
            else if(strcmp(node->data.stringData,"printc") == 0){
                node->type = PRINTC;
            }
            else if(strcmp(node->data.stringData,"const") == 0){
                node->type = CONST;
            }
            else if(strcmp(node->data.stringData,"and") == 0){
                node->type = AND;
            }else if(strcmp(node->data.stringData,"or") == 0){
                node->type = OR;
            }
            else if(strcmp(node->data.stringData,"not") == 0){
                node->type = NOT;  
            }
            else if(strcmp(node->data.stringData,"import") == 0){
                node->type = IMPORT;
            }else if(strcmp(node->data.stringData,"True") == 0){
                node->type = BOOLEAN;
                node->data.boolData = true;
            }
            else if(strcmp(node->data.stringData,"False") == 0){
                node->type = BOOLEAN;
                node->data.boolData = false;
            }
            else{
                node->type = IDENTIFIER;
            }
        }
        else if(isdigit(line[i])){
            double num = 0.0;
            while(i < n && isdigit(line[i])){
                num = num*10.0 + (line[i] - '0');
                i++;
            }
            if(i < n && line[i] == '.'){
                i++;
                double fract = 0.1;
                while(i < n && isdigit(line[i])){
                    num+= (line[i] - '0') * fract;
                    fract = fract/ 10.0;
                    i++;
                }
            }
            node->data.numData= num;
            node->type = NUMBER;
        }
        else if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/'){
            node->type = OPERATOR;
            node->data.charData = line[i];
            i++;
        }
        else if(line[i] == '='){
            node->type = EQUALS;
            i++;
        }else if(line[i] == '('){
            node->type  = L_PARAN;
            i++;
        }else if(line[i] == ')'){
            node->type = R_PARAN;
            i++;
        }
        else{
            printf("Python Error : Unidentified Token Found By Lexer \n");
            freeGC();
            exit(1);
        }
        
        node->next = NULL;
        if(head == NULL){
            head  = node;
            tail = node;
        }else{
            tail->next = node;
            tail = node;
        }
        
    }
    return head;
}

bool isHashFirstInLine(const char* line){
    int n = strlen(line);
    int i = 0;
    while(i < n && isspace(line[i])){
        i++;
    }
    return line[i] == '#';
}

typedef enum{
    V_STRING,
    V_NUMBER,
    V_BOOLEAN
} VariableType;
typedef struct Variable{
    VariableType type;
    char varname[20];
    bool isConstant;
    union{
        double numData;
        char stringData[256];
        bool boolData;
    } data;
    struct Variable* next;
} Variable;
Variable* createVariable(){
   Variable*  ptr = (Variable*) GC_create(sizeof(Variable)); 
    if(!ptr){
        printf("Python Error: Memory Allocation for Variable Storage Failed \n");
        freeGC();
        exit(1);
    }
    return ptr;
}
Variable* STORE = NULL;

void insertVariable(Variable* v){
    if(STORE == NULL){
        STORE = v;
    }else{
        Variable* current = STORE;
        while(current != NULL){
            if(strcmp(v->varname,current->varname) == 0){
                if(current->isConstant){
                    printf("Python Error: Trying to Modify A Constant Variable %s \n",current->varname);
                    freeGC();
                    exit(1);
                }
                if(v->type == BOOLEAN){
                    current->data.boolData = v->data.boolData;
                }else if(v->type == NUMBER){
                    current->data.numData = v->data.numData;
                }else if(v->type == STRING){
                    strcpy(current->data.stringData,v->data.stringData);
                }
                return;
            }
            current = current->next;
        }
        v->next = STORE;
        STORE = v;
    }
}
Variable* getVariable(char* varname){
    Variable* current = STORE;
        while(current != NULL){
            if(strcmp(varname,current->varname) == 0){
                return current;
            }
            current = current->next;
        }
        printf("Python Error: Usage of Undefined Variable \n");
        freeGC();
        exit(1);
}

typedef enum{
    NUMBER_NODE,
    OPERATOR_NODE,
    BOOLEAN_NODE,
    AND_NODE,
    OR_NODE,
    NOT_NODE,
    STRING_NODE,
} ASTNodeType;

typedef struct ASTNode{
    ASTNodeType type;
    union{
        char charData;
        double numData;
        bool boolData;
    } data;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* child;
} ASTNode;

typedef enum{
    BINARY_EXPRESSION,
    LOGICAL_EXPRESSION,
    STRING_EXPRESSION
} EXPRESSION_TYPE;

LexerNode* current = NULL;
void consume(){
    if(current){
        current=  current->next;
    }
}
ASTNode* parseAddSub();
ASTNode* parseMulDiv();
ASTNode* parseBinExp();

ASTNode* parseAndOr();
ASTNode* parseNot();
ASTNode* parseLogicExp();


ASTNode* parseBinExp(){
    if(current->type == NUMBER){
        ASTNode* node = GC_create(sizeof(ASTNode));
        node->data.numData =  current->data.numData;
        node->type = NUMBER_NODE;
        consume();
        return node;
    }else if(current->type == L_PARAN){
        consume();
        ASTNode* node = parseAddSub();
        if(current == NULL || current->type != R_PARAN){
            printf("Python Error: ) needed in Binary Expression \n");
            freeGC();
            exit(1);
        }
        consume();
        return node;
    }else{
        printf("Python Error: Unidentified Token found while Parsing Binary Expression %d : %c\n",current->type,current->data.charData);
        freeGC();
        exit(1);
    }
}
ASTNode* parseAddSub(){
    ASTNode* lhs = parseMulDiv();
    while(current != NULL && current->type == OPERATOR && (current->data.charData == '+' || current->data.charData == '-')){
        char op = current->data.charData;
        ASTNode* parent = GC_create(sizeof(ASTNode));
        parent->data.charData = op;
        parent->type = OPERATOR_NODE; 
        consume();
        ASTNode* rhs = parseMulDiv();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}
ASTNode* parseMulDiv(){
    ASTNode* lhs = parseBinExp();
    while(current != NULL && current->type == OPERATOR && (current->data.charData == '*' || current->data.charData == '/')){
        char op = current->data.charData;
        ASTNode* parent = GC_create(sizeof(ASTNode));
        parent->data.charData = op;
        parent->type = OPERATOR_NODE; 
        consume();
        ASTNode* rhs = parseBinExp();
        parent->left = lhs;
        parent->right = rhs;
        lhs = parent;
    }
    return lhs;
}

ASTNode* parseLogicExp(){
    if(current->type == BOOLEAN){
        ASTNode* node = GC_create(sizeof(ASTNode));
        node->type = BOOLEAN_NODE;
        node->data.boolData = current->data.boolData;
        consume();
        return node;
    }else if(current->type == L_PARAN){
        consume();
        ASTNode* node = parseAndOr();
        if(current == NULL || current->type != R_PARAN){
            printf("Python Error: ) needed in Logical Expression \n");
            freeGC();
            exit(1);
        }
        consume();
        return node;
    }else if(current->type == NUMBER){
           ASTNode* node = GC_create(sizeof(ASTNode));
           node->type = BOOLEAN_NODE;
           if(current->data.numData == 0){
            node->data.boolData = false;
           }else{
            node->data.boolData = true;
           }
            consume();
        return node;
    }else{
        printf("Python Error: Unidentified Token found while Parsing Logical Expression \n");
        freeGC();
        exit(1);
    }
}
ASTNode* parseAndOr(){
    ASTNode* lhs = parseNot();
    while(current != NULL &&  (current->type == AND || current->type == OR)){
        ASTNode* parent = GC_create(sizeof(ASTNode));
        if(current->type == AND) parent->type = AND_NODE;
        if(current->type == OR) parent->type = OR_NODE;
        consume();
        ASTNode* rhs = parseNot();
        parent->left = lhs;
        parent->right = rhs;
        
        lhs = parent;
    }
    return lhs;
}
ASTNode* parseNot(){
    while(current != NULL && current->type == NOT){
        ASTNode* parent = GC_create(sizeof(ASTNode));
        parent->type = NOT_NODE;
        consume();
        parent->child = parseLogicExp();
        return parent;
    }
    return parseLogicExp();
}
char* parseString(LexerNode* head){
    char *finalString = (char*) GC_create(256);
    finalString[0] = '\0';
    LexerNode* temp = head;
    bool canConcat = true;
    while(temp != NULL){
        if(temp->type == STRING && canConcat){   
            strcat(finalString,temp->data.stringData);
            canConcat = false;
        } 
        if(temp->type == OPERATOR && temp->data.charData == '+'){
            canConcat = true;
        }
        
        temp = temp->next;
   }
   return finalString;
}

double SOLVE_BINARY_EXPRESSION(ASTNode* root){
    if(root->type == NUMBER_NODE){
        return root->data.numData;
    }else{
        switch (root->data.charData)
        {
        case '+':
            return SOLVE_BINARY_EXPRESSION(root->left) + SOLVE_BINARY_EXPRESSION(root->right);
            break;
        case '-':
            return SOLVE_BINARY_EXPRESSION(root->left) - SOLVE_BINARY_EXPRESSION(root->right);
            break;
        case '*':
            return SOLVE_BINARY_EXPRESSION(root->left) * SOLVE_BINARY_EXPRESSION(root->right);
            break;
        case '/':
            if(SOLVE_BINARY_EXPRESSION(root->right) == 0){
                printf("Python Error: Division by Zero \n");
                freeGC();
                exit(1);
            }
            return SOLVE_BINARY_EXPRESSION(root->left) / SOLVE_BINARY_EXPRESSION(root->right);
            break;
        default:
            break;
        }
    }
}
double SOLVE_LOGICAL_EXPRESSION(ASTNode* root){
    if(root->type == BOOLEAN_NODE){
        return root->data.boolData;
    }else{
        switch (root->type)
        {
        case AND_NODE:
            return SOLVE_LOGICAL_EXPRESSION(root->left) && SOLVE_LOGICAL_EXPRESSION(root->right);
            break;
        case OR_NODE:
            return SOLVE_LOGICAL_EXPRESSION(root->left) || SOLVE_LOGICAL_EXPRESSION(root->right);
            break;
        case NOT_NODE:
            return !SOLVE_LOGICAL_EXPRESSION(root->child);
            break;
        default:
            break;
        }
    }
}
EXPRESSION_TYPE get_exp_type(LexerNode* start){
    bool isBinExp = false,isStrExp = false;
    LexerNode* current = start;
    while(current != NULL){
        if(current->type == BOOLEAN || current->type == AND ||  current->type == OR ||  current->type == NOT){
            return LOGICAL_EXPRESSION;
        }
        if(current->type == NUMBER || current->type == OPERATOR){
            isBinExp = true;
        }
        if(current->type == STRING){
            isStrExp = true;
        }
        current = current->next;
    }
    if(isBinExp && !isStrExp){
        return BINARY_EXPRESSION;
    }else{
        return STRING_EXPRESSION;
    }
    return BINARY_EXPRESSION;
}

void replaceVariablesInLexer(LexerNode* expStart){
    LexerNode* temp = expStart;
    while(temp != NULL){
        if(temp->type == IDENTIFIER){
            Variable* v = getVariable(temp->data.stringData);
            if(v->type ==  V_STRING){
                temp->type = STRING;
                strcpy(temp->data.stringData,v->data.stringData);
            }else if(v->type == V_BOOLEAN){
                temp->type = BOOLEAN;
                temp->data.boolData = v->data.boolData;
            }else if(v->type == V_NUMBER){
                temp->type = NUMBER;
                temp->data.numData = v->data.numData;
            }
        }
        temp = temp->next;
    }
}
void interpretFile(char* filename);
void LineParser(LexerNode* head){
    current = head;
    if((head->type == CONST && head->next->type == IDENTIFIER && head->next->next->type == EQUALS) || (head->type == IDENTIFIER && head->next->type == EQUALS)){
        bool isConstant = head->type == CONST ? true : false;
        char varname[256];
        LexerNode* expStart;
        if(isConstant){
            strcpy(varname, head->next->data.stringData);
            expStart = head->next->next->next;
        }else{
            strcpy(varname, head->data.stringData);
             expStart = head->next->next;
        }
        replaceVariablesInLexer(expStart);
        Variable* v = createVariable();
        strcpy(v->varname, varname);
        EXPRESSION_TYPE e = get_exp_type(expStart);
        current = expStart;
        if(e == BINARY_EXPRESSION){
            ASTNode* bin_root = parseAddSub();
            double result = SOLVE_BINARY_EXPRESSION(bin_root);
            v->data.numData = result;
            v->next = NULL;
            v->isConstant = isConstant;
            v->type = V_NUMBER;
        }
        else if(e == LOGICAL_EXPRESSION){
            ASTNode* logic_root = parseAndOr();
            bool result = SOLVE_LOGICAL_EXPRESSION(logic_root);
            v->type = V_BOOLEAN;
            v->data.boolData = result;
            v->isConstant = isConstant;
        }else if(e == STRING_EXPRESSION){
            char* str = parseString(expStart);
            v->type = V_STRING;
            v->isConstant = isConstant;
            strcpy(v->data.stringData,str);
        }else{
            printf("Python Error: Undefined Expression Type \n");
            freeGC();
            exit(1);
        }
        insertVariable(v);
    }else if(head->type == PRINT){
        if(head->next->type != L_PARAN){
            printf("Python Error : Print function is missing ( \n");
            freeGC();
            exit(1);
        }
        LexerNode* expStart = head->next->next;
        current = expStart;
        replaceVariablesInLexer(expStart);
        EXPRESSION_TYPE e = get_exp_type(expStart);
        if(e == BINARY_EXPRESSION){
            ASTNode* bin_root = parseAddSub();
            double result = SOLVE_BINARY_EXPRESSION(bin_root);
            if(floor(result)  == result){
                printf("%d\n",(int) result);
            }else{
                printf("%f\n",result);
            }
        }
        else if(e == LOGICAL_EXPRESSION){
            ASTNode* logic_root = parseAndOr();
            bool result = SOLVE_LOGICAL_EXPRESSION(logic_root);
            if(result) printf("True\n");
            else printf("False\n");
        }else if(e == STRING_EXPRESSION){
            char* str = parseString(expStart);
            printf("%s\n", str);
        }else{
            printf("Python Error: Undefined Expression Type \n");
            freeGC();
            exit(1);
        }
    }else if(head->type == PRINTC){
        if(head->next->type != L_PARAN){
            printf("Python Error : Print function is missing ( \n");
            freeGC();
            exit(1);
        }
        LexerNode* expStart = head->next->next;
        current = expStart;
        replaceVariablesInLexer(expStart);
        EXPRESSION_TYPE e = get_exp_type(expStart);
        if(e == BINARY_EXPRESSION){
            ASTNode* bin_root = parseAddSub();
            double result = SOLVE_BINARY_EXPRESSION(bin_root);
            if(floor(result)  == result){
                printf("%d",(int) result);
            }else{
                printf("%f",result);
            }
        }
        else if(e == LOGICAL_EXPRESSION){
            ASTNode* logic_root = parseAndOr();
            bool result = SOLVE_LOGICAL_EXPRESSION(logic_root);
            if(result) printf("True");
            else printf("False");
        }else if(e == STRING_EXPRESSION){
            char* str = parseString(expStart);
            printf("%s", str);
        }else{
            printf("Python Error: Undefined Expression Type \n");
            freeGC();
            exit(1);
        }
    }else if(head->type == IMPORT){
        LexerNode* fileIdentifier = head->next;
        char filename[100];
        strcpy(filename,fileIdentifier->data.stringData);
        strcat(filename,".py");
        interpretFile(filename);
    }else{
        printf("\n Python Error : Possible usage of unknown variables/built - un functions (Syntax Error) \n");
        freeGC();
        exit(1);
    }
    current = NULL;
}

void interpretFile(char* filename){
 FILE* fp  = fopen(filename,"r");
    if(fp == NULL){
        printf("Python Error : File Not Found For Interpretation \n");
        exit(1);
    }
    char line[512];

    while(fgets(line,512,fp) != NULL){
        line[strcspn(line, "\r\n")] = 0; 
        char* commentPos = strchr(line,'#');
        if(commentPos != NULL){
            *commentPos = '\0';
        }
        if(isHashFirstInLine(line) || strlen(line) == 0){
            continue;
        }
        LexerNode* LineLexerOutput =  (LexerNode*) LineLexer(line);
        LineParser(LineLexerOutput);
    }
    fclose(fp);
    
}
int main(int argCount,char* args[]){
    if(argCount > 2){
        printf("Python Error: Extra arguments are not allowed \n");
        freeGC();
        exit(1);
    }
    if(argCount < 2){
        printf("Python Error: File Name required for Interpretation \n");
    }
   char *filename = args[1];
   size_t len = strlen(filename);
   if(len >= 3){
        char temp[4];
        strncpy(temp,filename+len-3,3);
        temp[3] = '\0';
        if(strcmp(temp,".py") == 0){
            interpretFile(filename);
        } else{
            printf("Python Error: File is not of .py extension \n");
            freeGC();
            exit(1);
        }
   }else{
        printf("Python Error : File name is too short \n");
        freeGC();
        exit(1);
   }

   freeGC();
   return 0;
}