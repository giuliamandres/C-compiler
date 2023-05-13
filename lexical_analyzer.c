#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "stdarg.h"
#include "regex.h"

enum {ID, END, CHAR, ASSIGN, SEMICOLON, EQUAL, BREAK, DOUBLE, ELSE, FOR, IF, INT, RETURN, STRUCT, VOID, WHILE, CT_INT, CT_REAL, CT_CHAR, CT_STRING, COMMA, LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC, ADD, SUB, MUL, DIV, DOT, AND, OR, NOT, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ, SPACE, LINECOMMENT, COMMENT};
//    0   1     2       3       4       5       6       7       8   9    10   11    12      13     14     15    16      17      18          19       20     21    22    23          24      25    26    27  28   29    30   31   32  33   34    35      36   37     38          39      40      41          42

char *buffer = NULL;

typedef struct _Token {
    int code;
    union {
        char text[30]; //for string attributes
        long int i; //for int attributes
        double r; //for real attributes
    };
    int line; //the input file line
    struct _Token *next;
}Token;

Token *head = NULL;
Token *last = NULL;

Token *addToken(int code, int line){
    Token *tk = (Token*)malloc(sizeof(Token));
    tk->code = code;
    tk->next = NULL;
    tk->line = line;
    if(last){
        last->next = tk;
    }
    else{
        head = tk;
    }
    last = tk;
    return tk;
}

void tokenErr(const Token *tk, const char *fmt,...){
    va_list va;
    va_start(va, fmt);
    fprintf(stderr, "error in line %d ", tk->line);
    vfprintf(stderr, fmt, va);
    fputc('\n', stderr);
    va_end(va);
    exit(-1);

}

int line = 0;

int getNextToken(){
    char ch = 0;
    int currentState = 0;
    Token *tk = (Token *)malloc(sizeof(Token));
    //char *currentToken = (char *) malloc(20 * sizeof(char));
    char currentToken[20] = {'\0'};
    //char currentToken[20] = "";
    char *endptr = NULL;
    regex_t regex;
    int regularExp;

    while(1){
        ch = *buffer;
        //printf("ch %c\n", ch);
        switch (currentState) {
            case 0:
                //printf("reading");
                if(isalpha(ch) || ch == '_'){
                    printf("tok length %lu\n", strlen(currentToken));
                    buffer++;
                    currentState = 1;
                    strncat(currentToken, &ch, 1);
                    printf("tok length %lu\n", strlen(currentToken));
                }
                else if(ch == '='){
                    buffer++;
                    currentState = 3;
                }
                else if(ch == ' ' || ch == '\r' || ch == '\t'){
                    buffer++;
                }
                else if(ch == '\n'){
                    buffer++;
                    line++;
                }
                else if(ch == '\0'){
                    addToken(END, line);
                    return END;
                }
                else if(isdigit(ch)){
                    if(ch != '0'){
                        //printf("OK");
                        buffer++;
                        currentState = 6;
                        strncat(currentToken, &ch, 1);
                    }
                    else{
                        printf("state 0");
                        buffer++;
                        currentState = 8;
                        strncat(currentToken, &ch, 1);
                    }
                }
                else if(ch == 39){ //ASCII code for '
                    buffer++;
                    currentState = 24;
                    strncat(currentToken, &ch, 1);
                }
                else if(ch == '"'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 30;
                    break;
                }
                else if(ch == ','){
                    buffer++;
                    currentState = 35;
                }
                else if(ch == ';'){
                    buffer++;
                    currentState = 36;
                }
                else if(ch == '('){
                    buffer++;
                    currentState = 37;
                }
                else if(ch == ')'){
                    buffer++;
                    currentState = 38;
                }
                else if(ch == '['){
                    buffer++;
                    currentState = 39;
                }
                else if(ch == ']'){
                    buffer++;
                    currentState = 40;
                }
                else if(ch == '{'){
                    buffer++;
                    currentState = 41;
                }
                else if(ch == '}'){
                    buffer++;
                    currentState = 42;
                }
                else if(ch == '+'){
                    buffer++;
                    currentState = 43;
                }else if(ch == '-'){
                    buffer++;
                    currentState = 44;
                }
                else if(ch == '*'){
                    buffer++;
                    currentState = 45;
                }
                else if(ch == '/'){
                    buffer++;
                    currentState = 31;
                }
                else if(ch == '.'){
                    buffer++;
                    currentState = 47;
                }
                else if(ch == '&'){
                    buffer++;
                    currentState = 48;
                }
                else if(ch == '|'){
                    buffer++;
                    currentState = 49;
                }
                else if(ch == '!'){
                    buffer++;
                    currentState = 50;
                }
                else if(ch == '<'){
                    buffer++;
                    currentState = 51;
                }
                else if(ch == '>'){
                    buffer++;
                    currentState = 52;
                }
                else {
                    tokenErr(addToken(END, line), "invalid character");}
                break;
            case 1:
                if (isalnum(ch) || ch == '_'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                } else currentState = 2;
                break;
            case 2:
                //printf("current token length %lu string length %lu token %s\n", strlen(currentToken), strlen("void"), currentToken);
                if (strncmp(currentToken, "break", strlen("break")) == 0){
                    tk = addToken(BREAK, line);
                }
                else if(strncmp(currentToken, "char", strlen("char")) == 0){
                    printf("OK");
                    tk = addToken(CHAR, line);
                }
                else if(strncmp(currentToken, "double", strlen("double")) == 0){
                    tk = addToken(DOUBLE, line);
                }
                else if(strncmp(currentToken, "else", strlen("else")) == 0){
                    tk = addToken(ELSE, line);
                }
                else if(strncmp(currentToken, "for", strlen("for")) == 0){
                    tk = addToken(FOR, line);
                }
                else if(strncmp(currentToken, "if", strlen("if")) == 0){
                    tk = addToken(IF, line);
                }
                else if(strncmp(currentToken, "int", strlen("int")) == 0){
                    tk = addToken(INT, line);
                }
                else if(strncmp(currentToken, "return", strlen("return")) == 0){
                    tk = addToken(RETURN, line);
                }
                else if(strncmp(currentToken, "struct", strlen("struct")) == 0){
                    tk = addToken(STRUCT, line);
                }
                else if(strncmp(currentToken, "void", strlen("void")) == 0){
                    tk = addToken(VOID, line);
                }
                else if(strncmp(currentToken, "while", strlen("while")) == 0){
                    tk = addToken(WHILE, line);
                }
                else{
                    tk = addToken(ID, line);
                    strcpy(tk->text, currentToken);
                    printf("string %s\n", currentToken);
                }
                return tk->code;
            case 3:
                if (ch == '=')
                {
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 4;
                }
                else
                {
                    currentState = 5;
                }
                break;
            case 4:
                addToken(EQUAL, line);
                return EQUAL;
            case 5:
                addToken(ASSIGN, line);
                return ASSIGN;
            case 6:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else if(ch == '.'){
                    buffer++;
                    currentState = 15;
                    strncat(currentToken, &ch, 1);
                }
                else if(ch == 'e' || ch == 'E'){
                    buffer++;
                    currentState = 17;
                    strncat(currentToken, &ch, 1);
                }
                else{
                    currentState = 7;
                }
                break;
            case 7:
                tk = addToken(CT_INT, line);
                tk->i = atoi(currentToken);
                printf("digit %ld\n", tk->i);
                return CT_INT;
            case 8:
                printf("case 8");
                if(isdigit(ch) && ch != '8' && ch != '9'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 10;
                }
                else if(ch == 'x'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 12;
                }
                else if(ch == '.'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 9;
                } else{
                    currentState = 62;
                }
                break;
            case 9:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 16;
                }
                else{
                    tokenErr(addToken(END, line), "invalid character");
                }
                break;
            case 10:
                if(isdigit(ch) && ch != '8' && ch != '9'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else {
                    currentState = 11;
                }
                break;
            case 11: //problem with strtol
                addToken(CT_INT, line);
                //printf("token %s\n", currentToken);
                int octal = strtol(currentToken, &endptr, 8);
                //printf("end %s\n", endptr);
                printf("octal %d\n", octal);
                tk->i = octal;
                printf("%ld\n", tk->i);
                //printf("i %ld", tk->i);
                return CT_INT;
            case 12:
                if(isdigit(ch) || (ch >= 'a' && ch <='f') || (ch >= 'A' && ch <='F')){
                    currentState = 13;
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else if(!isdigit(ch) || (!(ch >= 'a' && ch <='f') && !(ch >= 'A' && ch <='F'))){
                    tokenErr(addToken(END, line), "invalid character");
                }
                break;
            case 13:
                if(isdigit(ch) || (ch >= 'a' && ch <='f') || (ch >= 'A' && ch <='F')){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else
                    currentState = 14;
                break;
            case 14:
                tk = addToken(CT_INT, line);
                tk->i = strtol(currentToken, NULL, 16);
                return CT_INT;
            case 15:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 16;
                }
                else if(ch == 'e' || ch == 'E'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 17;
                }
                else
                    tokenErr(addToken(END, line), "invalid character");
                break;
            case 16:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else if(ch == 'e' || ch =='E'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 17;
                }
                else
                    currentState = 23;
                break;
            case 17:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 19;
                }
                else if(ch == '+' || ch == '-'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 18;
                } else
                    tokenErr(addToken(END, line), "invalid character");
                break;
            case 18:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 21;
                }
                else
                    tokenErr(addToken(END, line), "invalid character");
                break;
            case 19:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else
                    currentState = 20;
                break;
            case 20:
                tk = addToken(CT_REAL, line);
                tk->r = atof(currentToken);
                printf("real1 %f", tk->r);
                return CT_REAL;
            case 21:
                if(isdigit(ch)){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                }
                else
                    currentState = 22;
                break;
            case 22:
                tk = addToken(CT_REAL, line);
                tk->r = atof(currentToken);
                printf("real2 %f\n", tk->r);
                return CT_REAL;
            case 23:
                addToken(CT_REAL, line);
                printf("token %s\n", currentToken);
                tk->r = atof(currentToken);
                printf("test %f\n", atof("0.5"));
                return CT_REAL;
            case 24:
                regularExp = regcomp(&regex, "[^'\\]", 0);
                if(regularExp){
                    printf("Could not compile regex");
                    return -1;
                }
                if(ch == '\\'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 25;
                }
                else if(!(regexec(&regex, &ch, 0, NULL, 0))){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 28;
                }
                else tokenErr(addToken(END, line), "invalid character");
                break;
            case 25:
                if(ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' || ch == 'v' || ch == 39 || ch == '?' || ch == '"' || ch == '\\' || ch == '0'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 26;
                }
                else tokenErr(addToken(END, line), "invalid character");
                break;
            case 26:
                if(ch == 39){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 27;
                }
                else tokenErr(addToken(END, line), "invalid character");
                break;
            case 27:
                tk = addToken(CT_CHAR, line);
                strcpy(tk->text, currentToken);
                return CT_CHAR;
            case 28:
                if(ch == 39){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 29;
                }
                else
                    tokenErr(addToken(END, line), "invalid character");
                break;
            case 29:
                tk = addToken(CT_CHAR, line);
                strcpy(tk->text, currentToken);
                return CT_CHAR;
            case 30:
                //return -1;
                printf("OK2 ");
                if(ch != '"'){
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    printf("curr %s\n", currentToken);
                    //return -1;
                }
                else{
                    buffer++;
                    strncat(currentToken, &ch, 1);
                    currentState = 33;
                }
                break;
            case 31:
                if(ch == '*'){
                    buffer++;
                    currentState = 34; //53
                }
                else if(ch == '/'){
                    buffer++;
                    currentState = 55; //55
                }
                else{
                    currentState = 32; //61
                }
                break;
            case 32:
                addToken(DIV, line);
                return DIV;
            case 33:
                tk = addToken(CT_STRING, line);
                strcpy(tk->text, currentToken);
                return CT_STRING;
            case 34:
                if(ch != '*'){
                    buffer++;
                }
                else{
                    buffer++;
                    currentState = 54;
                }
                break;
            case 35:
                addToken(COMMA, line);
                return COMMA;
            case 36:
                addToken(SEMICOLON, line);
                return SEMICOLON;
            case 37:
                addToken(LPAR, line);
                return LPAR;
            case 38:
                addToken(RPAR, line);
                return RPAR;
            case 39:
                addToken(LBRACKET, line);
                return LBRACKET;
            case 40:
                addToken(RBRACKET, line);
                return RBRACKET;
            case 41:
                addToken(LACC, line);
                return LACC;
            case 42:
                addToken(RACC, line);
                return RACC;
            case 43:
                addToken(ADD, line);
                return ADD;
            case 44:
                addToken(SUB, line);
                return SUB;
            case 45:
                addToken(MUL, line);
                return MUL;
            case 46:
                addToken(LESS, line);
                return LESS;
            case 47:
                addToken(DOT, line);
                return DOT;
            case 48:
                if(ch == '&'){
                    buffer++;
                    currentState = 56;
                } else tokenErr(addToken(END, line), "invalid character");
                break;
            case 49:
                if(ch == '|'){
                    buffer++;
                    currentState = 57;
                }
                else tokenErr(addToken(END, line), "invalid character");
                break;
            case 50:
                if(ch == '='){
                    buffer++;
                    currentState = 58;
                }
                else{
                    currentState = 53;
                }
                break;
            case 51:
                if(ch == '='){
                    buffer++;
                    currentState = 59;
                }
                else{
                    currentState = 46;
                }
                break;
            case 52:
                if(ch == '='){
                    buffer++;
                    currentState = 60;
                }
                else{
                    currentState = 61;
                }
                break;
            case 53:
                addToken(NOT, line);
                return NOT;
            case 54:
                if(ch == '/'){
                    buffer++;
                    currentState = 0;
                }
                else {
                    buffer++;
                }
                break;
            case 55:
                if(ch == '\n' || ch == '\r' || ch == '\0'){
                    buffer++;
                    currentState = 0;
                }
                else{
                    buffer++;
                }
                break;
            case 56:
                addToken(AND, line);
                return AND;
            case 57:
                addToken(OR, line);
                return OR;
            case 58:
                addToken(NOTEQ, line);
                return NOTEQ;
            case 59:
                addToken(LESSEQ, line);
                return LESSEQ;
            case 60:
                addToken(GREATEREQ, line);
                return GREATEREQ;
            case 61:
                addToken(GREATER, line);
                return GREATER;
            case 62:
                tk = addToken(CT_INT, line);
                tk->i = atoi(currentToken);
                return CT_INT;


        }
    }
}

void showTokens(){
    Token *curr = head;
    while (curr != NULL)
    {
        if(curr->code == 0){
            printf("TOKEN %d %s %d\n", curr->code, curr->text, curr->line);
        }
        else if(curr->code == 16){
            printf("TOKEN %d %ld %d\n", curr->code, curr->i, curr->line);
        }
        else if(curr->code == 17){
            printf("TOKEN %d %f %d\n", curr->code, curr->r, curr->line);
        }
        else if(curr->code == 18 || curr->code == 19){
            printf("TOKEN %d %s %d\n", curr->code, curr->text, curr->line);
        }
        else {
            printf("TOKEN %d %d\n", curr->code, curr->line);
        }
        curr = curr->next;
    }
}

void freeMemory(){
    Token *curr = head;
    while (curr != NULL)
    {
        free(curr);
        curr = curr->next;
    }
}

/*int main(){
    FILE *f = fopen("file.txt", "r");
    //char *buffer = 0;
    if(f == NULL)
        printf("Can t open the file");
    else{
        fseek(f, 0, SEEK_END);
        long length = ftell(f);
        printf("length %ld\n", length);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if(buffer){
            fread(buffer, 1, length, f);
            printf("%s\n", buffer);
        }
        fclose(f);
    }

    int token = getNextToken();
    printf("token code %d\n", token);
    while (token != 1){
        token = getNextToken();
        printf("token code %d\n", token);
    }
    showTokens();
    freeMemory();
    fun();

}*/