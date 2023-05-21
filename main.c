#include "stdio.h"
//#include "lexical_analyzer.c"
#include "syntactic_analyzer.c"


int main(){
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
    //freeMemory();
    //printTk();
    int sintactic = unit();
    printf("%d", sintactic);

}