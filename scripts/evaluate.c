#include <evaluate.h>
#include <parser.h>
#include <memoryHandler.h>
#include <error.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <variableRegistry.h>
#include <functionRegistry.h>
#include <math.h>
#include <callStack.h>
#include <globals.h>
#include <imports.h>
Value makeValue(){
    Value v;
    v.isReturnedValue = false;
    return v;
}
Value makeNone(){
    Value v;
    v.isReturnedValue= false;
    v.type = D_NONE;
    return v;
}

bool isTruthy(Value val){
    switch(val.type){
        case D_NUMBER:
            return val.data.numData != 0;
        case D_STRING:
            return strcmp((char*) val.data.stringData,"") != 0;
        case D_BOOLEAN:
            return val.data.boolData;
        case D_NONE:
            return false;
        default:
            return false;
    }
}
Value evaluate(astNode* node){
   if(node->type == AST_STRING){
     Value v = makeValue();
     MemNode* strObj = createMemNode(strlen(node->data.stringData)+1);
     v.data.stringData = strObj->ptr;
     strcpy(v.data.stringData,node->data.stringData);
     v.type = D_STRING;
     return v;     
   }
   else if(node->type == AST_INDEXED){
        Value v = makeValue();
        Value index = evaluate(node->index);
        if(strlen(node->data.stringData) <= index.data.numData){
            error("Index out of bounds",node->lineCount,INDEXING_ERROR);
        }
        if(index.type != D_NUMBER){
            error("Invalid Parameter For Indexing",node->lineCount,INDEXING_ERROR);
        }
        MemNode* strObj = createMemNode(3);
        v.data.stringData = strObj->ptr;
        if(index.data.numData != floor(index.data.numData) || index.data.numData < 0){
            error("Index Error , Wrong Parameter (Required : Number)",node->lineCount,INDEXING_ERROR);
        }
        v.data.stringData[0] = node->data.stringData[(int) index.data.numData];
        v.data.stringData[1] = '\0';
        v.type = D_STRING;
        return v;
   }
   else if(node->type == AST_IDENTIFIER_INDEXED){
        Variable* variable = getVariable(node->data.stringData);
        Value * var = variable->data;
        Value index = evaluate(node->index);
        if(strlen(var->data.stringData) <= index.data.numData){
            error("Index out of bounds",node->lineCount,INDEXING_ERROR);
        }
        if(index.type != D_NUMBER){
            error("Invalid Parameter For Indexing",node->lineCount,INDEXING_ERROR);
        }
        Value result;
        if(var->type != D_STRING){
            char errMsg[256];
            snprintf(errMsg,sizeof(errMsg),"Indexing is only allowed for strings. Check '%s' 's type (String Required)",variable->varName);
            error(errMsg,node->lineCount,INDEXING_ERROR);
        }
        result.type = D_STRING;
        MemNode* strObj = createMemNode(3);
        result.data.stringData = strObj->ptr;
        result.data.stringData[0] = var->data.stringData[(int) index.data.numData];
        result.data.stringData[1] = '\0';
        return result;
   }
   else if(node->type == AST_NUMBER){
    Value v;
    v.data.numData = node->data.numData;
    v.type = D_NUMBER;
    return v;
   }
   else if(node->type == AST_BOOLEAN){
    Value v;
    v.data.boolData = node->data.boolData;
    v.type = D_BOOLEAN;
    return v;
   }
   else if(node->type == AST_IDENTIFIER){
        Variable* variable = getVariable(node->data.stringData);
        Value * var = variable->data;
        Value result;
        if(var->type == D_STRING){
            result.type = D_STRING;
            MemNode* strObj = createMemNode(strlen((char*) var->data.stringData)+1);
            strcpy(strObj->ptr,(char*) var->data.stringData);
            result.data.stringData = strObj->ptr; 
        }else if(var->type == D_NUMBER){
            result.type = D_NUMBER;
            result.data.numData =  var->data.numData;
        }else if(var->type == D_BOOLEAN){
            result.type = D_BOOLEAN;
            result.data.boolData = var->data.boolData;
        }else{
            result.type = D_NONE;
        }
        return result;
    }else if(node->type == AST_NONE){
        Value v;
        v.type= D_NONE;
        return v;
    }else if(node->type == AST_OPERATOR){
        char op = node->data.charData;
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        switch(op){
            case '+':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type= D_NUMBER;
                    v.data.numData = left.data.numData + right.data.numData;
                }else if(left.type == D_STRING && right.type == D_STRING){
                    MemNode* node = createMemNode(strlen(left.data.stringData) + strlen(right.data.stringData) + 1);
                    v.data.stringData = node->ptr;
                    v.type = D_STRING;
                    strcpy(v.data.stringData,left.data.stringData);
                    strcat(v.data.stringData,right.data.stringData);
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData + right.data.boolData;
                }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.numData + right.data.boolData;
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData + right.data.numData;
                }
                else{
                    error("Incorrect Usage of '+' operator",node->lineCount,SYNTAX_ERROR);
                }
                break;
            case '-':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.numData - right.data.numData;
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData - right.data.boolData;
                }   
                else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.numData - right.data.boolData;
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData - right.data.numData;
                }
                else{
                    error("Incorrect Usage of '-' operator",node->lineCount,SYNTAX_ERROR);
                }
                break;
            case '*':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type= D_NUMBER;
                    v.data.numData = left.data.numData * right.data.numData;
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData * right.data.boolData;
                }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.numData * right.data.boolData;
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = left.data.boolData * right.data.numData;
                }
                else if(left.type == D_STRING && right.type == D_NUMBER){
                    if(right.data.numData <= 0 || floor(right.data.numData) != right.data.numData){
                        error("Usage of String Multiplication with a ZERO/Negative number is Illegal",node->lineCount,RUN_TIME_ERROR);
                    }
                    MemNode* node = createMemNode(strlen(left.data.stringData) * right.data.numData + 1);
                    v.data.stringData = (char*) node->ptr;
                    for(int i =0;i < right.data.numData;i++){
                        if(i == 0) strcpy(v.data.stringData,left.data.stringData);
                        else strcat(v.data.stringData,left.data.stringData);
                    }
                    v.type = D_STRING;
                }
                else if(right.type == D_STRING && left.type == D_NUMBER){
                    if(left.data.numData <= 0 || floor(left.data.numData) != left.data.numData){
                        error("Usage of String Multiplication with a ZERO/Negative number is Illegal",node->lineCount,RUN_TIME_ERROR);
                    }
                    MemNode* node = createMemNode(strlen(right.data.stringData) * left.data.numData + 1);
                    v.data.stringData = (char*) node->ptr;
                    for(int i =0;i < left.data.numData;i++){
                        if(i == 0) strcpy(v.data.stringData,right.data.stringData);
                        else strcat(v.data.stringData,right.data.stringData);
                    }
                    v.type = D_STRING;
                }
                else{
                    error("Incorrect Usage of '*' operator",node->lineCount,SYNTAX_ERROR);
                }
                break;
            case '/':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type= D_NUMBER;
                    if(right.data.numData == 0){
                        error("Division By Zero Error",node->lineCount,DIVISION_BY_ZERO_ERROR);
                    }
                    v.data.numData = left.data.numData / right.data.numData;
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    if(right.data.boolData == false){
                        error("False is 0, /False",node->lineCount,DIVISION_BY_ZERO_ERROR);
                    }
                    v.data.numData = left.data.boolData / right.data.boolData;
                }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    if(right.data.boolData == false){
                        error("False is 0, /False",node->lineCount,SYNTAX_ERROR);
                    }
                    v.data.numData = left.data.numData / right.data.boolData;
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    if(right.data.numData == 0){
                        error("Division By Zero Error (Denominator Int = 0)",node->lineCount,SYNTAX_ERROR);
                    }
                    v.data.numData = left.data.boolData / right.data.numData;
                }
                else{
                    error("Incorrect Usage of '/' operator",node->lineCount,RUN_TIME_ERROR);
                }
                break;
            case '%':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = (int) left.data.numData % (int) right.data.numData;
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = (int) left.data.boolData % (int) right.data.boolData;
                }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = (int) left.data.numData % (int) right.data.boolData;
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = (int) left.data.boolData % (int) right.data.numData;
                }
                else{
                    error("Incorrect Usage of '%' (modulus) operator",node->lineCount,RUN_TIME_ERROR);
                }
                break;
            case '^':
                if(left.type == D_NUMBER && right.type == D_NUMBER){
                    v.type= D_NUMBER;
                    v.data.numData = pow(left.data.numData,right.data.numData);
                }
                else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = pow(left.data.boolData,right.data.boolData);
                }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
                    v.type = D_NUMBER;
                    v.data.numData = pow(left.data.numData,right.data.boolData);
                }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
                    v.type = D_NUMBER;
                    v.data.numData = pow(left.data.boolData,right.data.numData);
                }
                else{
                    error("Incorrect Usage of '^' (exponent) operator",node->lineCount,RUN_TIME_ERROR);
                }
                break;
        }
        return v;
    }else if(node->type == AST_AND){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        v.data.boolData = isTruthy(left) && isTruthy(right);  
        return v;
    }
    else if(node->type == AST_OR){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        v.data.boolData = isTruthy(left) || isTruthy(right);
        return v;
    }
    else if(node->type == AST_NOT){
        Value child = evaluate(node->child);
        Value v;
        v.type = D_BOOLEAN;
        v.data.boolData = !isTruthy(child);
        return v;
    }
    else if(node->type == AST_GT){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience",node->lineCount,RUN_TIME_ERROR);
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData > right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData > right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData > right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData > right.data.boolData;
        }else{
            error("Incorrect Usage of '>' operator",node->lineCount,RUN_TIME_ERROR);
        }   
        return v;
    }
    else if(node->type == AST_LT){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience",-node->lineCount,RUN_TIME_ERROR);
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData < right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData < right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData < right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData < right.data.boolData;
        }else{
            error("Incorrect Usage of '<' operator",node->lineCount,RUN_TIME_ERROR);
        } 
        return v;
    }
    else if(node->type == AST_GEQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience",node->lineCount,RUN_TIME_ERROR);
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData >= right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData >= right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData >= right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData >= right.data.boolData;
        }else{
            error("Incorrect Usage of '>=' operator",node->lineCount,RUN_TIME_ERROR);
        }
        
        
        return v;
    }else if(node->type == AST_LEQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience",node->lineCount,RUN_TIME_ERROR);
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData <= right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData <= right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData <= right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData <= right.data.boolData;
        }else{
            error("Incorrect Usage of '<=' operator",node->lineCount,RUN_TIME_ERROR);
        }
        
        
        return v;
    }else if(node->type == AST_EQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING && right.type == D_STRING){
            v.data.boolData =  strcmp(left.data.stringData,right.data.stringData) == 0;
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData == right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData == right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData == right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData == right.data.boolData;
        }
        else if(left.type == D_NONE && right.type == D_NONE){
            v.data.boolData = true;
        }
        else if(left.type == D_NONE || right.type == D_NONE){
            v.data.boolData = false;
        }
        else{
            error("Incorrect Usage of '==' operator",node->lineCount,RUN_TIME_ERROR);
        }
        return v;
    }
    else if(node->type == AST_NEQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        v.type = D_BOOLEAN;
        if(left.type == D_STRING && right.type == D_STRING){
            v.data.boolData =  strcmp(left.data.stringData,right.data.stringData) != 0;
        }else if(left.type == D_NUMBER && right.type == D_NUMBER){
            v.data.boolData = left.data.numData != right.data.numData;
        }
        else if(left.type == D_BOOLEAN && right.type == D_BOOLEAN ){
            v.data.boolData = left.data.boolData != right.data.boolData;
        }else if(left.type == D_BOOLEAN && right.type == D_NUMBER){
            v.data.boolData = left.data.boolData != right.data.numData;
        }else if(left.type == D_NUMBER && right.type == D_BOOLEAN){
            v.data.boolData = left.data.numData != right.data.boolData;
        }
        else if(left.type == D_NONE && right.type == D_NONE){
            v.data.boolData = false;
        }else if(left.type == D_NONE || right.type == D_NONE){
            v.data.boolData = true;
        }
        else{
            error("Incorrect Usage of '!=' operator",node->lineCount,RUN_TIME_ERROR);
        }
        return v;
    }
    else if(node->type == AST_ASSIGNMENT){
        Value varNameValue = evaluate(node->left);
        char* varName = varNameValue.data.stringData;
        Value val  = evaluate(node->right);
        MemNode* valObj = createMemNode(sizeof(Value));
        *((Value*) valObj->ptr) = val;
        Variable* v = createVariable(varName,val.type,valObj);
        v->isConstant = node->isConstant;
        setVariable(v);
        return makeNone();
    }else if(node->type == AST_INT){
        Value val = evaluate(node->child);
        Value toReturn;
        toReturn.type = D_NUMBER;
        if(val.type == D_NUMBER ){
            toReturn.data.numData = val.data.numData;
        }else if(val.type == D_BOOLEAN){
            toReturn.data.numData = val.data.boolData;
        }
        else if(val.type == D_STRING){
            toReturn.data.numData = atoi(val.data.stringData);
        }
        else{
            error("Unexpected Argument passed into int() function (Check Datatype)",node->lineCount,RUN_TIME_ERROR);
        }
        return toReturn;
    }
    else if(node->type == AST_BOOL){
        Value v = evaluate(node->child);
        Value toReturn;
        toReturn.type = D_BOOLEAN;
        if(isTruthy(v)){
            toReturn.data.boolData = true;
        }else{
            toReturn.data.boolData = false;
        }
        
        return toReturn;
    }
    else if(node->type == AST_STR){
        Value v = evaluate(node->child);
        Value toReturn;
        toReturn.type = D_STRING;
        if(v.type == D_STRING){
            return v;
        }else if(v.type == D_BOOLEAN){
            if(v.data.boolData){
                MemNode* node = createMemNode(5);
                toReturn.data.stringData = node->ptr;
                strcpy(toReturn.data.stringData,"True");
            }else{
                MemNode* node = createMemNode(6);
                toReturn.data.stringData = node->ptr;
                strcpy(toReturn.data.stringData,"False");
            }
        }
        else if(v.type == D_NONE){
            MemNode* node = createMemNode(5);
            toReturn.data.stringData  = node->ptr;
            strcpy(toReturn.data.stringData,"None");
        }else if(v.type == D_NUMBER){
            if(v.data.numData == floor(v.data.numData)){
                MemNode* node = createMemNode(100);
                toReturn.data.stringData = node->ptr;
                int n = (int) v.data.numData;
                snprintf(toReturn.data.stringData,100,"%d",n);
            }else{
                 MemNode* node = createMemNode(100);
                toReturn.data.stringData = node->ptr;
                snprintf(toReturn.data.stringData,100,"%g",v.data.numData);
            }
        }
        return toReturn;
    }
    else if(node->type == AST_LEN){
        astNode* val = node->child;
        Value inValue = evaluate(val);
        Value toReturn;
        if(inValue.type == D_STRING){
            toReturn.data.numData = strlen(inValue.data.stringData);
            toReturn.type = D_NUMBER;
        }else{
            error("len() does not support number/boolean/None as a valid parameter",node->lineCount,RUN_TIME_ERROR);
        }
        return toReturn;
    }
    else if(node->type == AST_TYPE){
        astNode* val = node->child;
        Value inValue = evaluate(val);
        Value toReturn;
        toReturn.type = D_STRING;
        MemNode* strObj = createMemNode(10);
        if(inValue.type == D_STRING){
            strcpy(strObj->ptr,"STRING");
        }else if(inValue.type == D_NUMBER){
            strcpy(strObj->ptr,"NUMBER");
        }else if(inValue.type == D_BOOLEAN){
            strcpy(strObj->ptr,"BOOLEAN");
        }else{
            strcpy(strObj->ptr,"NONE");
        }
        toReturn.data.stringData  = strObj->ptr;
        return toReturn;
    }
    else if(node->type == AST_PRINT){
        astNode* current = node->child;
        Value v;
        while(current != NULL){
            v =evaluate(current);
            switch(v.type){
                case D_NUMBER:
                    if(floor(v.data.numData) == v.data.numData){
                        printf("%d",(int) floor(v.data.numData));
                    }else{
                        printf("%g",v.data.numData);
                    }
                    printf(" ");
                    fflush(stdout);
                    break;
                case D_STRING:
                    printf("%s",(char*) v.data.stringData);
                    printf(" ");
                    break;
                case D_BOOLEAN:
                    if(v.data.boolData == true){
                        printf("True ");
                    }else{
                        printf("False ");
                    }
                    break;
                case D_NONE:
                    printf("None ");
                    break;
                default:
                    break;
            }
            current = current->thenNext;
        }
         printf("\n");
        return makeNone();
    }else if(node->type == AST_INPUT){
        Value inputAsker = evaluate(node->child);
        Value toReturn;
        toReturn.type = D_STRING;
        MemNode* buf = createMemNode(1024);
        toReturn.data.stringData = buf->ptr;
        switch(inputAsker.type){
            case D_STRING:
                printf(inputAsker.data.stringData);
                fgets(toReturn.data.stringData,1024,stdin);
                size_t len = strlen(toReturn.data.stringData);
                char** toReturnStr = &toReturn.data.stringData;
                if(len > 0 && (*toReturnStr)[len - 1] == '\n'){
                    (*toReturnStr)[len - 1] = '\0';
                }
                break;
            case D_NUMBER:
                if(floor(inputAsker.data.numData) == inputAsker.data.numData){
                    printf("%d",(int) floor(inputAsker.data.numData));
                }else{
                    printf("%g",inputAsker.data.numData);
                }
                scanf("%s",(char*) toReturn.data.stringData);
                break;
            case D_BOOLEAN:
                if(inputAsker.data.boolData){
                    printf("True");
                }else{
                    printf("False");
                }
                scanf("%s",(char*) toReturn.data.stringData);
                break;
            case D_NONE:
                printf("None");
                scanf("%s",(char*) toReturn.data.stringData);
                break;
            default:
                break;
        }
        return toReturn;
    }
    else if(node->type == AST_FOR){
        int end_value = evaluate(node->end_value).data.numData;
        MemNode* varObj = createMemNode(sizeof(Variable));
        Value val = evaluate(node->start_value);
        val.type = D_NUMBER;
        val.isReturnedValue = false;
        Variable* iterator = createVariable(node->data.stringData,val.type,varObj);
        (* iterator->data) = val;
        setVariable(iterator);
        for(int i = iterator->data->data.numData;i < end_value;i++){
            astNode* stmt = node->thenBlock;
            Value result;
            iterator->data->data.numData++;
            while(stmt){
                result = evaluate(stmt);
                if(result.isReturnedValue){
                    return result;
                }
                stmt = stmt->thenNext;
            }
        }
        return makeNone();
    }
    else if(node->type == AST_WHILE){
        Value condition = evaluate(node->child);
        while(isTruthy(condition)){
            astNode* stmt = node->thenBlock;
            Value result;
            while(stmt){
                result = evaluate(stmt);
                if(result.isReturnedValue){
                    return result;
                }
                stmt = stmt->thenNext;
            }
            condition = evaluate(node->child);
        }
        return makeNone();
    }else if(node->type == AST_IF){
        Value condition = evaluate(node->child);
        if(isTruthy(condition)){
            astNode* current = node->thenBlock;
            Value result;
            while(current != NULL){
                result = evaluate(current);
                if(result.isReturnedValue){
                    return result;
                }
                current = current->thenNext;
            }
        }else{
            if(node->nextBlock){
                Value result = evaluate(node->nextBlock);
                if(result.isReturnedValue){
                    return result;
                }
            }else if(node->elseBlock != NULL){
                astNode* current = node->elseBlock->thenBlock;
                Value result;
                while(current != NULL){
                    result = evaluate(current);
                    if(result.isReturnedValue){
                        return result;
                    }
                    current = current->thenNext;
                }
            }
        }
        return makeNone();
    }else if(node->type == AST_FUNCTION_DECLARATION){
        Function* f = createFunction();
        MemNode* strObj = createMemNode(strlen(node->data.stringData)+1);
        strcpy(strObj->ptr,node->data.stringData);
        f->functionName = strObj->ptr;
        astNode* paramaters = node->param;
        Param* head = NULL;
        Param* tail = NULL;
        while(paramaters != NULL){
            char* paramName = paramaters->data.stringData;
            Param * param = createParam(paramName);
            if(head == NULL){
                head = param;
                tail= param;
            }else{
                tail->next = param;
                tail = param;
            }
            f->param_length++;
            paramaters = paramaters->nextParam;
        }
        f->params = head;
        f->thenBlock = node->thenBlock;
        setFunction(f);
        return makeNone();
    }
    else if(node->type == AST_FUNCTION_CALL){
        Value param_values_arr[node->param_length];
        astNode* param_values = node->param;
        Function* function = getFunction(node->data.stringData);
        Param* params = function->params;
        CallStackNode* callStackNode = createCallStackNode();
        callStackNode->function = function;
        if(function->param_length != node->param_length){
            error("Invalid Parameters, Check if all parameters are given",node->lineCount,FUNCTION_ERROR);
        }
        int i = 0;
        while(param_values){
            param_values_arr[i] = evaluate(param_values);
            param_values = param_values->nextParam;
            i++;
        }
        pushCallStackNode(callStackNode);
        i = 0;
        while(params){
            char* varName = params->paramName;
            Value val = param_values_arr[i];
            MemNode* valObj = createMemNode(sizeof(Value));
            *((Value*) valObj->ptr) = val;
            Variable* v = createVariable(varName,val.type,valObj);
            setVariable(v);
            params = params->next;
            i++;
        }
        astNode* stmt = function->thenBlock;
        if(stmt == NULL){
                error("Function cannot be empty, Check indentation and ensure function logic is present",node->lineCount,FUNCTION_ERROR);
        }
        Value result;
        while(stmt != NULL){
            result = evaluate(stmt);
            if(result.isReturnedValue && result.isReturnedValue == true){
                result.isReturnedValue = false;
                popCallStackNode();
                return result;
            }
            stmt = stmt->thenNext;
        }
        popCallStackNode();
        return makeNone();
    }
    else if(node->type == AST_RETURN){
        Value v = evaluate(node->child);
        v.isReturnedValue = true;
        return v;
    }else if(node->type == AST_LOAD){
        char* fileName = node->data.stringData;
        if(isAlreadyLoaded(fileName)) return makeNone();
        GlobalsCopy copy = copy_state();
        clear_states();
        interpret(fileName);
        restore_state(copy);
    }
    return makeNone();
}


void Execute(){
    astNode* cur = ast_root;
    while(cur){
        evaluate(cur);
        cur = cur->astChain;
    }
}