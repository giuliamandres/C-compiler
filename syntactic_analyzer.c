#include "stdio.h"
#include "lexical_analyzer.c"

Token *currentTk = NULL;
Token *consumedTk = NULL;
//Token *startTk = NULL;

int expr();

int consume(int code){
    if(currentTk->code == code){
        consumedTk = currentTk;
        currentTk = currentTk->next;
        return 1;
    }
    return 0;
}


int exprPostFix1(){
    Token *startTk = currentTk;
    printf("EXPRPOSTFIX1\n");
    if(consume(LBRACKET)){
        if(expr()){
            if(consume(RBRACKET)){
                if(exprPostFix1()){
                    return 1;
                }
            }
            else error("Missing right parenthesis\n");
        }
    }
    currentTk = startTk;
    if(consume(DOT)){
        if(consume(ID)){
            if(exprPostFix1()){
                return 1;
            }
            else error("Missing ID");
        }
    }
    return 1;
}

int exprPrimary();

//exprPostfix: exprPostfix LBRACKET expr RBRACKET | exprPostfix DOT ID | exprPrimary => exprPostfix = exprPrimary exprPostfix1; exprPostfix1 = LBRACKET expr RBRACKET exprPostfix1 | DOT ID exprPostfix1
int exprPostFix(){
    printf("EXPROPOSTFIX\n");
    if(exprPrimary()){
        if(exprPostFix1()){
            return 1;
        }
    }
    return 0;
}

//exprUnary: (SUB | NOT ) exprUnary | exprPostfix
int exprUnary(){
    Token *startTk = currentTk;
    printf("EXPRUNARY\n");
    if(consume(SUB)){
        if(exprUnary()){
            return 1;
        }
        error("Expected exprUnary");
    }
    currentTk = startTk;
    if(consume(NOT)){
        if(exprUnary()){
            return 1;
        }
        error("Expected exprUnary");
    }
    currentTk = startTk;
    if(exprPostFix()){
        return 1;
    }
    return 0;
}

int typeBase(){
    printf("TYPEBASE\n");
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
    }
    return 0;
}


int typeName(){
    if(typeBase()){
        if(arrayDecl()){}
        return 1;
    }
    return 0;
}

//exprCast: LPAR typeName RPAR exprCast | exprUnary
int exprCast(){
    Token *startTk = currentTk;
    printf("EXPRCAST\n");
    if(consume(LPAR)){
        if(typeName()){
            if(consume(RPAR)){
                if(exprCast()){
                    return 1;
                }
            } else error("Missing right parenthesis");
        }
    }
    currentTk = startTk;
    if(exprUnary()){
        return 1;
    }
    return 0;
}

int exprMul1(){
    printf("EXPRMUL1\n");
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
    return 1;
}

//exprMul: exprMul ( MUL | DIV ) exprCast | exprCast => exprMul = exprCast exprMul1; exprMul1 = ( MUL | DIV ) exprCast exprMul1
int exprMul(){
    printf("EXPRMUL\n");
    if(exprCast()){
        if(exprMul1()){
            return 1;
        } else error("Expected exprMul1");
    }
    return 0;
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
    return 1;
}

//exprAdd: exprAdd ( ADD | SUB ) exprMul | exprMul => exprAdd = exprMul exprAdd1; exprAdd1 = ( ADD | SUB ) exprMul exprAdd1
int exprAdd(){
    printf("EXPRADD\n");
    if(exprMul()){
        if(exprAdd1()){
            return 1;
        } else error("Invalid exprAdd1");
    }
    return 0;
}


int exprRel1(){
    if(consume(LESS)){
        printf("LESS\n");
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
        printf("GREATEREQ\n");
        if(exprAdd()){
            if(exprRel1()){
                return 1;
            } else error("Invalid exprRel1");
        } else error("Invalid exprAdd");
    }
    return 1;
}

//exprRel: exprRel ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd | exprAdd => exprRel = exprAdd exprRel1; exprRel1 = ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd exprRel1
int exprRel(){
    printf("EXPRREL\n");
    if(exprAdd()){
        if(exprRel1()){
            return 1;
        } else error("Invalid exprRel1");
    }
    return 0;
}


int exprEq1(){
    printf("EXPREQ1\n");
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
    printf("EXPREQ\n");
    if(exprRel()){
        if(exprEq1()){
            return 1;
        } else error("Invalid exprEq1");
    }
    return 0;
}

//exprAnd: exprAnd AND exprEq | exprEq; => exprAnd = exprEq exprAnd1; exprAnd1 = AND exprEq exprAnd1
int exprAnd1(){
    printf("EXPRAND1\n");
    if(consume(AND)){
        printf("CONSUMED AND\n");
        if(exprEq()){
            if(exprAnd1()){
                return 1;
            }
        }
    }
    return 1;
}

int exprAnd(){
    printf("EXPRAND\n");
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
                return 1;
            }
        }
    }
    return 1;
}

//exprOr: exprOr OR exprAnd | exprAnd => exprOr = exprAnd exprOr1; exprOr1 = OR exprAnd exprOr1
int exprOr(){
    printf("EXPROR\n");
    if(exprAnd()){
        if(exprOr1()){
            return 1;
        }
    }
    return 0;
}

//exprAssign: exprUnary ASSIGN exprAssign | exprOr
int exprAssign(){
    Token *startTk = currentTk;
    printf("start tk %d\n", startTk->code);
    printf("EXPRAASIGN\n");
    if(exprUnary()){
        if(consume(ASSIGN)){
            printf("CONSUMED =\n");
            if(exprAssign()){
                return 1;
            } else error("Invalid exprAssign\n");
        }
    }
    printf("start token %d\n", startTk->code);
    currentTk = startTk;
    printf("curr tk %d\n", currentTk->code);
    if(exprOr()){
        return 1;
    }
    return 0;
}

int expr(){
    printf("EXPR\n");
    if(exprAssign()){
        printf("OKexpr\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}

int stm();

int ruleIF(){
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
    return 0;
}

int ruleWhile(){
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
    return 0;
}

int ruleFor(){
    printf("RULEFOR\n");
    if(consume(FOR)){
        if(consume(LPAR)){
            printf("CONSUMED LPAR\n");
            if(expr()){}
            if(consume(SEMICOLON)){
                printf("CONSUMED SEMICOLON1\n");
                if(expr()){}
                if(consume(SEMICOLON)){
                    printf("CONSUMED SEMICOLON2\n");
                    if(expr()){}
                    if(consume(RPAR)){
                        printf("CONSUMED RIGHTPAR\n");
                        if(stm()){
                            printf("RULEFOR OK1\n");
                            return 1;
                        }
                    } else tokenErr(currentTk, "Missing right parenthesis");
                } else tokenErr(currentTk, "Missing semicolon");
            } else tokenErr(currentTk, "Missing semicolon");
        } else tokenErr(currentTk, "Missing left parenthesis");
    }
    return 0;
}

int ruleBreak(){
    if(consume(BREAK)){
        if(consume(SEMICOLON)){
            return 1;
        } else tokenErr(currentTk, "Missing semicolon");
    }
    return 0;
}

int ruleReturn(){
    if(consume(RETURN)){
        if(expr()) {}
        if(consume(SEMICOLON)){
            return 1;
        } else tokenErr(currentTk, "Missing semicolon");
    }
    return 0;
}

int declVar(){
    printf("Var\n");
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){}
            while (1){
                if(consume(COMMA)){
                    if(consume(ID)){
                        if(arrayDecl()){}
                    } else tokenErr(currentTk, "Missing or invalid ID");
                }
                else break;
            }
            if(consume(SEMICOLON)){
                return 1;
            } else tokenErr(currentTk, "Missing semicolon");
        }
        tokenErr(currentTk, "Missing or invalid ID");
    }
    return 0;
}

int declStruct(){
    printf("Struct");
    if(consume(STRUCT)){
        if(consume(ID)){
            if(consume(LACC)){
                while (1){
                    if(declVar()){}
                    else break;
                }
                if(consume(RACC)){
                    if(consume(SEMICOLON)){
                        return 1;
                    } else tokenErr(currentTk, "Missing semicolon");
                } else tokenErr(currentTk, "Missing }");
            } else tokenErr(currentTk, "Missing {");
        } else tokenErr(currentTk, "Missing or invalid ID");
    }
    return 0;
}


int stmCompound();

int stm(){
    printf("STM\n");
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
        }else tokenErr(currentTk, "Missing semicolon\n");
    } else if(consume(SEMICOLON)){
        printf("SEMICOLON");
        return 1;
    }
    if(stmCompound()){
        return 1;
    }
    return 0;
}

int stmCompound(){
    printf("STMCOMPOUND\n");
    if(consume(LACC)){
        printf("LACC\n");
        while (1){
            if(declVar()){ printf("declVar ok\n");}
            else if(stm()){ printf("stm ok\n");}
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
    return 0;
}

int declFunc(){
    printf("function");
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
    if(consume(VOID)){//verify if it s type void
        printf("VOID\n");
        if(consume(ID)){
            printf("ID");
            if(consume(LPAR)){
                printf("LPAR\n");
                if(funcArg()){
                    printf("funcarg\n");
                    while (1){
                        if(consume(COMMA)){
                            if(funcArg()){}
                        }
                        else break;
                    }
                }
                if(consume(RPAR)){
                    printf("RPAR");
                    if(stmCompound()){
                        printf("stmcompound\n");
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
    printf("EXPRPRIMARY\n");
    //currentTk = head;
    printf("%d %s\n", currentTk->code, currentTk->text);
    if(consume(ID)){
        printf("ID\n");
        if(consume(LPAR)){
            printf("LPAR\n");
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
                printf("EXPR OK\n");
                return 1;
            }
            else error("Missing Right parenthesis");
        }
        printf("exprPrimary ok\n");
        return 1;
    }
    if(consume(CT_INT)){
        return 1;
    }
    if(consume(CT_REAL)){
        return 1;
    }
    if(consume(CT_CHAR)){
        printf("CHAR\n");
        return 1;
    }
    if(consume(CT_STRING)){
        printf("STRING\n");
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
    printf("RETURN 0\n");
    return 0;
}


int unit(){
    currentTk = head;
    printf("token code %d\n", currentTk->code);
    while(1){
        if(declStruct()){ printf("Struct ok\n");}
        else if(declFunc()){printf("Function OK\n");}
        else if(declVar()){ printf("variable ok\n");}
        else break;
    }
    if(consume(END)){
        return 1;
    }
    return 0;
}