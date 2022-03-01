%{
#include <stdlib.h>
#include <stdio.h>
void yyerror(char *s);
%}
%token tIF tELSE tWHILE tOPAR tCPAR tOCUR tCCUR tOBRA tCBRA tRET tPV tASSIGN tADD tMUL tGT tLT tMIN tAND tOR tEQ tINT tCONST tVOID
%token <text> tTEXT
%token <nb> tNB
%start Compilo
%%
Compilo	:	Fonction;
Fonction	:	tINT ;
%%
