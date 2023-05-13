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

//ID (LPAR ( expr ( COMMA expr )* )? RPAR )? | CT_INT | CT_REAL | CT_CHAR | CT_STRING | LPAR expr RPAR
int exprPrimary(){
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
        }
    }
    return 0;

}