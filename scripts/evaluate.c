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
 
void printValue(Value v){
    switch(v.type){
        case D_NUMBER:
            if(floor(v.data.numData) == v.data.numData){
                printf("%d",(int) floor(v.data.numData));
            }else{
                printf("%g",v.data.numData);
            }
            fflush(stdout);
            break;
        case D_STRING:
            printf("%s",(char*) v.data.stringData);
            break;
        case D_BOOLEAN:
            if(v.data.boolData == true){
                printf("True");
            }else{
                printf("False");
            }
            break;
        case D_NONE:
            printf("None");
            break;
        default:
        break;
    }
    printf(" ");
}

double toNumber(Value val){
    switch(val.type){
        case D_NUMBER:
            return val.data.numData;
            break;
        case D_BOOLEAN:
            return val.data.boolData;
            break;
        default:
            return val.data.numData;
            break;
    }
}
double strictToNumber(Value val,int lineCount){
    // api for division with checks
    double result = toNumber(val);
    if(result == 0){
        error("Division By Zero Error",lineCount,DIVISION_BY_ZERO_ERROR);
    }
    return result;
}

char* cloneString(char* str){
    int size = strlen(str) + 1;
    MemNode* strObj = createMemNode(size);
    strcpy(strObj->ptr,str);
    return strObj->ptr;
}


Value makeNone(){
    Value v;
    v.isReturnedValue= false;
    v.type = D_NONE;
    return v;
}

Value makeNumber(double num){
    Value v;
    v.type = D_NUMBER;
    v.data.numData = num;
    return v;
}

Value makeBoolean(bool boolean){
    Value v;
    v.type = D_BOOLEAN;
    v.data.boolData = boolean;
    return v;
}

Value makeString(char* str){
    Value v;
    v.type = D_STRING;
    v.data.stringData = cloneString(str);
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
Value runStatments(astNode* node){
    astNode* cur = node;
    Value result;
    while(cur){
        result = evaluate(cur);
        if(result.isReturnedValue){
            return result;
        }
        cur = cur->thenNext;
    }
    return makeNone();
}
Value evaluate(astNode* node){
   if(node->type == AST_STRING){
        Value v;
        if(node->isIndexed){
            char s[2];
            Value indexVal = evaluate(node->nextIndex);
            if(indexVal.type != D_NUMBER) error("Indexing a string with a non-number data type is illegal",node->lineCount,RUN_TIME_ERROR);
            int index = indexVal.data.numData;
            int n = strlen(node->data.stringData);
            if(index < 0) index = n + index;
            if(index < 0 || index >= n){
                error("Invalid index for string indexing",node->lineCount,RUN_TIME_ERROR);
            }
            s[0] = node->data.stringData[index];
            s[1] = '\0';
            v = makeString(s);
            if(node->nextIndex && node->nextIndex->nextIndex) error("Indexing of a string is one dimensional",node->lineCount,RUN_TIME_ERROR);
        }else{
            v = makeString(node->data.stringData);
        }
        return v;     
   }
   else if(node->type == AST_NUMBER){
        Value v = makeNumber(node->data.numData);
        return v;
   }
   else if(node->type == AST_BOOLEAN){
        Value v = makeBoolean(node->data.boolData);
        return v;
   }
   else if(node->type == AST_UPLUS){
        Value v = evaluate(node->child);
        return v; 
   }else if(node->type == AST_UMINUS){
        Value v = evaluate(node->child);
        v.data.numData = -v.data.numData;
        return v;
   }
   else if(node->type == AST_IDENTIFIER){
        Variable* variable = getVariable(node->data.stringData);
        Value * var = variable->data;
        Value result;
        if(node->isIndexed && (var->type != D_STRING)){
            error("Indexing an object which is neither a list nor a string is illegal",node->lineCount,RUN_TIME_ERROR);
        }
        // logic for indexing an identifier (ie variable)
        if(node->isIndexed){
            if(var->type == D_STRING){
                char s[2];
                Value indexVal = evaluate(node->nextIndex);
                if(indexVal.type != D_NUMBER) error("Invalid datatype for an index",node->lineCount,RUN_TIME_ERROR);
                int index = indexVal.data.numData;
                int n = strlen(var->data.stringData);
                if(index < 0) index = n + index;
                if(index < 0 || index >= n) error("Invalid index for indexing a string",node->lineCount,RUN_TIME_ERROR);
                s[0] =  var->data.stringData[index];
                s[1] = '\0';
                result = makeString(s);
                return result;
            }
        }

        if(var->type == D_STRING){
            result = makeString(var->data.stringData); 
        }else if(var->type == D_NUMBER){
            result = makeNumber(var->data.numData);
        }else if(var->type == D_BOOLEAN){
            result = makeBoolean(var->data.boolData);
        }else{
            result.type = D_NONE;
        }
        return result;
    }else if(node->type == AST_NONE){
        return makeNone();
    }else if(node->type == AST_OPERATOR){
        char op = node->data.charData;
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        switch(op){
            case '+':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber(toNumber(left) + toNumber(right));
                }
                else if(left.type == D_STRING && right.type == D_STRING){
                    char result[strlen(left.data.stringData) + strlen(right.data.stringData) + 1];
                    strcpy(result,left.data.stringData);
                    strcat(result,right.data.stringData);
                    v = makeString(result);
                } 
                else{
                    error("Incorrect usage of '+' operator",node->lineCount,SYNTAX_ERROR);
                }
                break;
            case '-':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber(toNumber(left) - toNumber(right));
                }
                else{
                    error("Incorrect Usage of '-' operator",node->lineCount,SYNTAX_ERROR);
                }
                break;
            case '*':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber(toNumber(left) * toNumber(right));
                }
                else if(left.type == D_STRING && right.type == D_NUMBER){
                    if(right.data.numData <= 0 || floor(right.data.numData) != right.data.numData){
                        error("String replication is illegal with 0 or negative numbers",node->lineCount,RUN_TIME_ERROR);
                    }
                    char result[strlen(left.data.stringData) * (int) right.data.numData + 1];
                    for(int i =0;i < right.data.numData;i++){
                        if(i == 0) strcpy(result,left.data.stringData);
                        else strcat(result,left.data.stringData);
                    }
                    v = makeString(result);
                }
                else if(right.type == D_STRING && left.type == D_NUMBER){
                    if(left.data.numData <= 0 || floor(left.data.numData) != left.data.numData){
                        error("String replication is illegal with 0 or negative numbers",node->lineCount,RUN_TIME_ERROR);
                    }
                    char result[strlen(right.data.stringData) * (int) left.data.numData + 1];
                    for(int i =0;i < left.data.numData;i++){
                        if(i == 0) strcpy(result,right.data.stringData);
                        else strcat(result,right.data.stringData);
                    }
                    v = makeString(result);
                }
                else{
                    error("Incorrect Usage of '*' operator",node->lineCount,SYNTAX_ERROR);
                }
            break; 
            case '/':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber(toNumber(left) / strictToNumber(right,node->lineCount));
                }
                else{
                    error("Incorrect Usage of '/' operator",node->lineCount,RUN_TIME_ERROR);
                }
                break;
            case '%':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber((int) toNumber(left) % (int) strictToNumber(right,node->lineCount));
                }
                else{
                    error("Incorrect Usage of '%' (modulus) operator",node->lineCount,RUN_TIME_ERROR);
                }
                break;
            case '^':
                if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeNumber(pow(toNumber(left),toNumber(right)));
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
        Value v = makeBoolean(isTruthy(left) && isTruthy(right));  
        return v;
    }
    else if(node->type == AST_OR){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v = makeBoolean(isTruthy(left) || isTruthy(right));
        return v;
    }
    else if(node->type == AST_NOT){
        Value child = evaluate(node->child);
        Value v = makeBoolean(!isTruthy(child));
        return v;
    }
    else if(node->type == AST_IN){
        Value rhs = evaluate(node->right);
        Value lhs = evaluate(node->left);
        Value v;
        v.type = D_BOOLEAN;
        if(rhs.type != D_STRING){
            error("'in' operator requires a string in the right-hand side",node->lineCount,RUN_TIME_ERROR);
        }
        if(lhs.type != D_STRING){
            error("'in' operator requires a string in the left-hand side",node->lineCount,RUN_TIME_ERROR);
        }
        if(strstr(rhs.data.stringData,lhs.data.stringData) != NULL){
            v.data.boolData = true;
        }else{
            v.data.boolData = false;
        }
        return v;
    }
    else if(node->type == AST_GT){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience :(",node->lineCount,RUN_TIME_ERROR);
        }else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) > toNumber(right));
                }
        else{
            error("Incorrect Usage of '>' operator",node->lineCount,RUN_TIME_ERROR);
        }
        return v;
    }
    else if(node->type == AST_LT){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience :(",node->lineCount,RUN_TIME_ERROR);
        }else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) < toNumber(right));
                }
        else{
            error("Incorrect Usage of '<' operator",node->lineCount,RUN_TIME_ERROR);
        }
        return v;
    }
    else if(node->type == AST_GEQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience :(",node->lineCount,RUN_TIME_ERROR);
        }else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) >= toNumber(right));
                }
        else{
            error("Incorrect Usage of '>=' operator",node->lineCount,RUN_TIME_ERROR);
        }
        return v;
    }else if(node->type == AST_LEQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        if(left.type == D_STRING || right.type == D_STRING){
            error("String Comparison is not yet added. Sorry for the inconvenience :(",node->lineCount,RUN_TIME_ERROR);
        }else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) <= toNumber(right));
                }
        else{
            error("Incorrect Usage of '<=' operator",node->lineCount,RUN_TIME_ERROR);
        }
        
        
        return v;
    }else if(node->type == AST_EQ){
        Value left = evaluate(node->left);
        Value right = evaluate(node->right);
        Value v;
        if(left.type == D_STRING && right.type == D_STRING){
            v = makeBoolean(strcmp(left.data.stringData,right.data.stringData) == 0);
        }else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) == toNumber(right));
        }
        else if(left.type == D_NONE && right.type == D_NONE){
            v = makeBoolean(true);
        }
        else if(left.type == D_NONE || right.type == D_NONE){
            v = makeBoolean(false);
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
            v = makeBoolean(strcmp(left.data.stringData,right.data.stringData) != 0);
        }
        else if((left.type == D_NUMBER && right.type == D_NUMBER) || 
                    (left.type == D_BOOLEAN && right.type == D_BOOLEAN) || 
                    (left.type == D_NUMBER && right.type == D_BOOLEAN) ||
                    (left.type == D_BOOLEAN && right.type == D_NUMBER)
                ){
                    v = makeBoolean(toNumber(left) != toNumber(right));
        }
        else if((left.type == D_NONE && right.type != D_NONE) || (left.type != D_NONE && right.type == D_NONE)){
            v = makeBoolean(true);
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
            toReturn.data.numData = (int) val.data.numData;
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
        Value toReturn = makeBoolean(isTruthy(v));
        return toReturn;
    }
    else if(node->type == AST_STR){
        Value v = evaluate(node->child);
        Value toReturn;
        if(v.type == D_STRING){
            return v;
        }else if(v.type == D_BOOLEAN){
            if(v.data.boolData){
                toReturn = makeString("True");
            }else{
                toReturn = makeString("False");
            }
        }
        else if(v.type == D_NONE){
            toReturn = makeString("None");
        }else if(v.type == D_NUMBER){
            if(v.data.numData == floor(v.data.numData)){
                char str[100];
                int n = (int) v.data.numData;
                snprintf(str,100,"%d",n);
                toReturn = makeString(str);
            }else{
                char str[100];
                snprintf(str,100,"%g",v.data.numData);
                toReturn = makeString(str);
            }
        }
        return toReturn;
    }
    else if(node->type == AST_LEN){
        astNode* val = node->child;
        Value inValue = evaluate(val);
        Value toReturn;
        if(inValue.type == D_STRING){
            toReturn = makeNumber(strlen(inValue.data.stringData));
        }else{
            error("len() does not support number/boolean/None as a valid parameter",node->lineCount,RUN_TIME_ERROR);
        }
        return toReturn;
    }
    else if(node->type == AST_TYPE){
        astNode* val = node->child;
        Value inValue = evaluate(val);
        Value toReturn;
        if(inValue.type == D_STRING){
            toReturn = makeString("STRING");
        }else if(inValue.type == D_NUMBER){
            toReturn = makeString("NUMBER");
        }else if(inValue.type == D_BOOLEAN){
            toReturn = makeString("BOOLEAN");
        }else{
            toReturn = makeString("NONE");
        }
        return toReturn;
    }
    else if(node->type == AST_PRINT){
        astNode* current = node->child;
        Value v;
        while(current != NULL){
            v = evaluate(current);
            printValue(v);
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
        if(node->isStringLoop){
            Value stringToTraverse;
            if(node->isVariableTraversal){
                Value variable_rhs = evaluate(node->child);
                if(variable_rhs.type != D_STRING){
                    error("For Loop requires a string object to traverse",node->lineCount,RUN_TIME_ERROR);
                }
                stringToTraverse = variable_rhs;
            }else{
                stringToTraverse = evaluate(node->child);
            }
            int stringLength = strlen(stringToTraverse.data.stringData);
            if(stringToTraverse.type != D_STRING){
                error("For Loop requires a string object to traverse",node->lineCount,RUN_TIME_ERROR);
            }
            MemNode* iteratorMemObj = createMemNode(sizeof(Variable));
            Value v;
            v.type = D_STRING;
            v.data.stringData = createMemNode(2)->ptr;
            v.data.stringData[0] = stringToTraverse.data.stringData[0];
            v.data.stringData[1] = '\0';
            Variable* iterator = createVariable(node->data.stringData,D_STRING,iteratorMemObj);
            *(iterator->data) = v;
            setVariable(iterator);
            // run the for loop
            Value result;
            for(int i = 0;i < stringLength;i++){
                v.data.stringData[0] = stringToTraverse.data.stringData[i];
                result = runStatments(node->thenBlock);
                if(result.isReturnedValue) return result;
            }
        }else{
            // get ranges
            Value range_start = evaluate(node->range_start);
            Value range_end = evaluate(node->range_end);
            Value range_skip = evaluate(node->range_skip);;
            // create the iterator variable
            MemNode* iteratorMemObj = createMemNode(sizeof(Variable));
            Value v;
            v.type = D_NUMBER;
            v.data = range_start.data;
            Variable* iterator = createVariable(node->data.stringData,D_NUMBER,iteratorMemObj);
            *(iterator->data) = v;
            setVariable(iterator);
            // run the for loop
            Value result;
            int start= iterator->data->data.numData;
            if(start > range_end.data.numData){
                for(int i = start;i > range_end.data.numData;i += range_skip.data.numData){ 
                    result = runStatments(node->thenBlock);
                    if(result.isReturnedValue) return result;
                    iterator->data->data.numData += range_skip.data.numData;
                }
            }else{
                for(int i = start;i < range_end.data.numData;i += range_skip.data.numData){ 
                    result = runStatments(node->thenBlock);
                    if(result.isReturnedValue) return result;
                iterator->data->data.numData += range_skip.data.numData;
            }
        }
        }
        return makeNone();
    }
    else if(node->type == AST_WHILE){
        Value condition = evaluate(node->child);
        Value result;
        while(isTruthy(condition)){
            result = runStatments(node->thenBlock);
            if(result.isReturnedValue) return result;
            condition = evaluate(node->child);
        }
        return makeNone();
    }else if(node->type == AST_IF){
        Value condition = evaluate(node->child);
        Value result;
        if(isTruthy(condition)){
            result = runStatments(node->thenBlock);
            if(result.isReturnedValue) return result;
        }else{
            if(node->nextBlock){
                Value result = evaluate(node->nextBlock);
                if(result.isReturnedValue){
                    return result;
                }
            }else if(node->elseBlock != NULL){
                Value result = runStatments(node->elseBlock->thenBlock);
                if(result.isReturnedValue) return result;
            }
        }
        return makeNone();
    }else if(node->type == AST_FUNCTION_DECLARATION){
        Function* f = createFunction();
        char* str = cloneString(node->data.stringData);
        f->functionName = str;
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
            if(result.isReturnedValue){
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
    }
    else if(node->type == AST_KILL){
        int status = removeVariable(node->data.stringData);
        // variable to remove  not found
        if(status == 0){
            status = removeFunction(node->data.stringData);
        }
        // if no variable/function of such name was removed
        if(status == 0){
            error("Unknown variable/function provided",node->lineCount,RUN_TIME_ERROR);
        }
        return makeNone();
    }
    else if(node->type == AST_MOUNT){
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