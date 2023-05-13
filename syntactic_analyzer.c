#include "stdio.h"
#include "lexical_analyzer.c"

Token *currentTk = NULL;
Token *consumedTk = NULL;

void printTk(){
    currentTk = head;
    while (currentTk != NULL){
        printf("code %d\n", currentTk->code);
        currentTk = currentTk->next;
    }
}


int consume(int code){
    if(currentTk->code == code){
        consumedTk = currentTk;
        currentTk = currentTk->next;
        return 1;
    }
    return 0;
}

int expr(){
    return 1;
}



int exprPostFix1(){
    if(consume(LBRACKET)){
        if(expr()){
            if(consume(RBRACKET)){
                if(exprPostFix1()){
                    return 1;
                }Í
            }
            else error("Missing right parenthesis\n");
        }
        else error("Expected expr");
    }
    if(consume(DOT)){
        if(consume(ID)){
            if(exprPostFix1()){
                return 1;
            }
            else error("Missing ID");
        }
    }
    return 0;
}

//exprPostfix: exprPostfix LBRACKET expr RBRACKET | exprPostfix DOT ID | exprPrimary => exprPostfix = exprPrimary exprPostfix1; exprPostfix1 = LBRACKET expr RBRACKET exprPostfix1 | DOT ID exprPostfix1
int exprPostFix(){
    if(exprPrimary()){
        if(exprPostFix1()){
            return 1;
        }
    }
    return 0;
}

//exprUnary: (SUB | NOT ) exprUnary | exprPostfix
int exprUnary(){
    if(consume(SUB)){
        if(exprUnary()){
            return 1;
        }
        if(exprPostFix()){
            return 1;
        }
        error("Expected exprUnary or exprPostFix");
    }
    if(consume(NOT)){
        if(exprUnary()){
            return 1;
        }
        if(exprPostFix()){
            return 1;
        }
        error("Expected exprUnary or exprPostFix");
    }
    return 0;
}

int typeName(){
    return 1;
}

//exprCast: LPAR typeName RPAR exprCast | exprUnary
int exprCast(){
    if(consume(LPAR)){
        if(typeName()){
            if(consume(RPAR)){
                if(exprCast()){
                    return 1;
                }
                if(exprUnary()){
                    return 1;
                }
            }
            else error("Missing right parenthesis");
        }
        else error("Expected typeName");
    }
    return 0;
}


//exprPrimary: ID (LPAR ( expr ( COMMA expr )* )? RPAR )? | CT_INT | CT_REAL | CT_CHAR | CT_STRING | LPAR expr RPAR
int exprPrimary(){
    currentTk = head;
    Token *startTk = currentTk;
    if(consume(ID)){
        if(consume(LPAR)){
            if(expr()){
                while (1){
                    if(consume(COMMA)){
                        if(expr()){
                            printf("expr ok");
                        }
                        else error("invalid expr after COMMA");
                    }
                    else break;
                }
            }
            if(consume(RPAR)){
                return 1;
            }
            else error("Missing Right parenthesis");
        }
        return 1;
    }
    if(consume(CT_INT)){
        return 1;
    }
    if(consume(CT_REAL)){
        return 1;
    }
    if(consume(CT_CHAR)){
        return 1;
    }
    if(consume(CT_STRING)){
        return 1;
    }
    if(consume(LPAR)){
        if(expr()){
            if(consume(RPAR)){
                return 1;
            }
            else error("Missing right parenthesis after expression");
        }
    }
    return 0;
}