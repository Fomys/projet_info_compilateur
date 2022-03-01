%{
#include <stdlib.h>
#include <stdio.h>
int var[26];
void yyerror(char *s);
%}
%union { int nb; char var; }
%token tFL tEGAL tPO tPF tSOU tADD tDIV tMUL tERROR
%token <nb> tNB
%token <var> tID
%type <nb> Expr DivMul Terme
%start Calculatrice
%%
Calculatrice :	  Calcul Calculatrice | Calcul ;
Calcul :	  Expr tFL { printf("> %d\n", $1); }
		| tID tEGAL Expr tFL { var[(int)$1] = $3; } ;
Expr :		  Expr tADD DivMul { $$ = $1 + $3; }
		| Expr tSOU DivMul { $$ = $1 - $3; }
		| DivMul { $$ = $1; } ;
DivMul :	  DivMul tMUL Terme { $$ = $1 * $3; }
		| DivMul tDIV Terme { $$ = $1 / $3; }
		| Terme { $$ = $1; } ;
Terme :		  tPO Expr tPF { $$ = $2; }
		| tID { $$ = var[$1]; }
		| tNB { $$ = $1; } ;
%%
void yyerror(char *s) { fprintf(stderr, "%s\n", s); }
int main(void) {
  printf("Calculatrice\n"); // yydebug=1;
  yyparse();
  return 0;
}
