#include "stdio.h"
#include "lexical_analyzer.c"

Token *currentTk = NULL;
Token *consumedTk = NULL;
Token *startTk = NULL;

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

//exprUnary: (SUB | NOT ) exprUnary | exprPostfix ?? left recursion
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

int typeBase(){
    if(consume(INT)){
        return 1;
    }
    if(consume(DOUBLE)){
        return 1;
    }
    if(consume(CHAR)){
        return 1;
    }
    if(consume(STRUCT)){
        if(consume(ID)){
            return 1;
        }
    }
    return 0;
}

int arrayDecl(){
    if(consume(LBRACKET)){
        if(expr()){}
        if(consume(RBRACKET)){
            return 1;
        } else tokenErr(currentTk, "Missing right bracket ]");
    } else tokenErr(currentTk, "Missing left bracket [ ");
    return 0;
}


int typeName(){
    if(typeBase()){
        if(arrayDecl()){}
        return 1;
    }
}

//exprCast: LPAR typeName RPAR exprCast | exprUnary
int exprCast(){
    if(consume(LPAR)){
        if(typeName()){
            if(consume(RPAR)){
                if(exprCast()){
                    return 1;
                }
            }
            else error("Missing right parenthesis");
        }
        else error("Expected typeName");
    }
    if(exprUnary()){
        return 1;
    }
    return 0;
}

int exprMul1(){
    if(consume(MUL)){
        if(exprCast()){
            if(exprMul1()){
                return 1;
            } else error("Expected exprMul1");
        } else error("Expected exprCast");
    }
    if(consume(DIV)){
        if(exprCast()){
            if(exprMul1()){
                return 1;
            } else error("Expected exprMul1");
        } else error("Expected exprCast");
    }
    return 0;
}

//exprMul: exprMul ( MUL | DIV ) exprCast | exprCast => exprMul = exprCast exprMul1; exprMul1 = ( MUL | DIV ) exprCast exprMul1
int exprMul(){
    if(exprCast()){
        if(exprMul1()){
            return 1;
        }
        else error("Expected exprMul1");
    }
    return 0;
}

//exprAdd: exprAdd ( ADD | SUB ) exprMul | exprMul => exprAdd = exprMul exprAdd1; exprAdd1 = ( ADD | SUB ) exprMul exprAdd1
int exprAdd(){
    if(exprMul()){
        if(exprAdd1()){
            return 1;
        } else error("Invalid exprAdd1");
    }else error("Invalid exprMul");
}

int exprAdd1(){
    if(consume(ADD)){
        if(exprMul()){
            if(exprAdd1()){
                return 1;
            } else error("Invalid exprAdd1");
        } else error("Invalid exprMul");
    }
    if(consume(SUB)){
        if(exprMul()){
            if(exprAdd1()){
                return 1;
            } else error("Invalid exprAdd1");
        } else error("Invalid exprMul");
    }
    return 0;
}


int exprRel1(){
    if(consume(LESS)){
        if(exprAdd()){
            if(exprRel1()){
                return 1;
            } else error("Invalid exprRel1");
        } else error("Invalid exprAdd");
    }
    if(consume(LESSEQ)){
        if(exprAdd()){
            if(exprRel1()){
                return 1;
            } else error("Invalid exprRel1");
        } else error("Invalid exprAdd");
    }
    if(consume(GREATER)){
        if(exprAdd()){
            if(exprRel1()){
                return 1;
            } else error("Invalid exprRel1");
        } else error("Invalid exprAdd");
    }
    if(consume(GREATEREQ)){
        if(exprAdd()){
            if(exprRel1()){
                return 1;
            } else error("Invalid exprRel1");
        } else error("Invalid exprAdd");
    }
    return 0;
}

//exprRel: exprRel ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd | exprAdd => exprRel = exprAdd exprRel1; exprRel1 = ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd exprRel1
int exprRel(){
    if(exprAdd()){
        if(exprRel1()){
            return 1;
        } else error("Invalid exprRel1");
    } else error("Invalid exprAdd");
}


int exprEq1(){
    if(consume(EQUAL)){
        if(exprRel()){
            if(exprEq1()){
                return 1;
            } else error("Invalid exprEq1");
        } else error("Invalid exprRel");
    }
    if(consume(NOTEQ)){
        if(exprRel()){
            if(exprEq1()){
                return 1;
            } else error("Invalid exprEq1");
        } else error("Invalid exprRel");
    }
    return 1;
}

//exprEq: exprEq ( EQUAL | NOTEQ ) exprRel | exprRel => exprEq = exprRel exprEq1; ExprEq1 = ( EQUAL | NOTEQ ) exprRel exprEq1
int exprEq(){
    if(exprRel()){
        if(exprEq1()){
            return 1;
        } else error("Invalid exprEq1");
    } else error("Invalid exprRel");
}

//exprAnd: exprAnd AND exprEq | exprEq; => exprAnd = exprEq exprAnd1; exprAnd1 = AND exprEq exprAnd1
int exprAnd1(){
    if(consume(AND)){
        if(exprEq1()){
            if(exprAnd1()){
                return 1;
            }
        }
    }
    return 0;
}

int exprAnd(){
    if(exprEq()){
        if(exprAnd1()){
            return 1;
        }
    }
    return 0;
}

int exprOr1(){
    if(consume(OR)){
        if(exprAnd()){
            if(exprOr1()){
                return 1
            }
        }
    }
    return 0;
}

//exprOr: exprOr OR exprAnd | exprAnd => exprOr = exprAnd exprOr1; exprOr1 = OR exprAnd exprOr1
int exprOr(){
    if(exprAnd()){
        if(exprOr1()){
            return 1;
        }
    }
    return 0;
}

//exprAssign: exprUnary ASSIGN exprAssign | exprOr
int exprAssign(){
    if(exprUnary()){
        if(consume(ASSIGN)){
            if(exprAssign()){
                return 1;
            }
        } else error("Missing assign expression");
    }
    if(exprOr()){
        return 1;
    }
}

int expr(){
    if(exprAssign()){
        return 1;
    }
    return 0;
}

int ruleIF(){
    startTk = currentTk;
    if(consume(IF)){
        if(consume(LPAR)){
            if(expr()){
                if(consume(RPAR)){
                    if(stm()){
                        if(consume(ELSE)){
                            if(stm()){
                                printf("OK");
                            }
                        }
                        return 1;
                    }
                } else error("Missing right parenthesis");
            } else error("Invalid expr");
        } else error("Missing left parenthesis");
    }
    currentTk = startTk;
    return 0;
}

int ruleWhile(){
    startTk = currentTk;
    if(consume(WHILE)){
        if(consume(LPAR)){
            if(expr()){
                if(consume(RPAR)){
                    if(stm()){
                        return 1;
                    }
                } else tokenErr(currentTk, "Missing right parenthesis");
            } else tokenErr(currentTk, "Invalid expr");
        } else tokenErr(currentTk, "Missing left parenthesis");
    }
    currentTk = startTk;
    return 0;
}

int ruleFor(){
    startTk = currentTk;
    if(consume(FOR)){
        if(consume(LPAR)){
            if(expr()){}
            if(consume(SEMICOLON)){
                if(expr()){}
                if(consume(SEMICOLON)){
                    if(expr()){}
                    if(consume(RPAR)){
                        if(consume(stm())){
                            return 1;
                        }
                    } else tokenErr(currentTk, "Missing right parenthesis");
                } else tokenErr(currentTk, "Missing semicolon");
            } else tokenErr(currentTk, "Missing semicolon");
        } else tokenErr(currentTk, "Missing left parenthesis");
    }
    currentTk = startTk;
    return 0;
}

int ruleBreak(){
    startTk = currentTk;
    if(consume(BREAK)){
        if(consume(SEMICOLON)){
            return 1;
        } else tokenErr(currentTk, "Missing semicolon");
    }
    currentTk = startTk;
    return 0;
}

int ruleReturn(){
    startTk = currentTk;
    if(consume(RETURN)){
        if(expr()) {}
        if(consume(SEMICOLON)){
            return 1;
        } else tokenErr(currentTk, "Missing semicolon");
    }
    currentTk = startTk;
    return 0;
}

int declVar(){
    return 1;
}

int stmCompound();

int stm(){
    if(stmCompound()){
        return 1;
    }
    if(ruleIF()){
        return 1;
    }
    if(ruleWhile()){
        return 1;
    }
    if(ruleFor()){
        return 1;
    }
    if(ruleBreak()){
        return 1;
    }
    if(ruleReturn()){
        return 1;
    }
    if(expr()){
        if(consume(SEMICOLON)){
            return 1;
        }
    } else if(consume(SEMICOLON)){
        return 1;
    }
    return 0;
}

int stmCompound(){
    if(consume(LACC)){
        while (1){
            if(declVar()){}
            else if(stm()){}
            else break;
        }
        if(consume(RACC)){
            return 1;
        } else tokenErr(currentTk, "Missing } or syntax error");
    }
    return 0;
}



int funcArg(){
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){}
            return 1;
        } else tokenErr(currentTk, "Invalid or missing ID");
    }
}

int declFunc(){
    if(typeBase()){ //verify if it s type int
        if(consume(MUL)){}
        if(consume(ID)){
            if(consume(LPAR)){
                if(funcArg()){
                    while (1){
                        if(consume(COMMA)){
                            if(funcArg()){}
                        }
                        else break;
                    }
                }
                if(consume(RPAR)){
                    if(stmCompound()){
                        return 1;
                    }
                } else tokenErr(currentTk, "Missing right parenthesis");
            } else tokenErr(currentTk, "Missing left parenthesis");
        } else tokenErr(currentTk, "Invalid or missing ID");
    }
    if(consume(VOID)){ //verify if it s type void
        if(consume(ID)){
            if(consume(LPAR)){
                if(funcArg()){
                    while (1){
                        if(consume(COMMA)){
                            if(funcArg()){}
                        }
                        else break;
                    }
                }
                if(consume(RPAR)){
                    if(stmCompound()){
                        return 1;
                    }
                } else tokenErr(currentTk, "Missing right parenthesis");
            } else tokenErr(currentTk, "Missing left parenthesis");
        } else tokenErr(currentTk, "Invalid or missing ID");
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