#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex_analiz.c"


int main(int argc, char **argv)
{
    char inBuf[10001];
    FILE *in;

    tokens = NULL;
    lastToken = NULL;
    line = 1;

    if((in=fopen(argv[1],"r"))==NULL)
        error("Eroare la deschiderea fisierului");

    int n = fread(inBuf,1,1000,in);
    inBuf[n] = '\0';
    fclose(in);

    pCrtCh = inBuf;
    while(getNextToken() != END) {}
    
    afisare();

    if(unit(tokens))
        printf("Syntax OK\n");
    else printf("Syntax ERROR\n");

    return 0;
}