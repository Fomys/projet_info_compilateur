%{
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"
#include "y.tab.h"

unsigned int offset;
struct AST_location yylloc;

#define YY_USER_ACTION         \
  offset += yyleng;            \
  yylloc.last_line = yylineno; \
  yylloc.last_column = offset;
%}

%option noyywrap

%%
"if"	{return tIF;}
"else"	{return tELSE;}
"while"	{return tWHILE;}
"("	{return tOPAR;}
")"	{return tCPAR;}
"{"	{return tOCUR;}
"}"	{return tCCUR;}
"return"	{return tRET;}
";"	{return tPV;}
","	{return tCOM;}
[ \t\n]	{}
"="	{return tASSIGN;}
"+"	{return tADD;}
"*"	{return tMUL;}
">"	{return tGT;}
"<"	{return tLT;}
"-"	{return tMIN;}
"=="	{return tEQ;}
"fn"	{return tFN;}
"print" {return tPRINT;}
"/"	{return tDIV;}
-?[0-9]+	{
    yylval.node = AST_node_alloc(yylloc);
    AST_node_integer_initialize(yylval.node, atoi(yytext));
    return tNB;
}
[a-z_]+	{
    yylval.identifier = malloc(strlen(yytext));
    strcpy(yylval.identifier, yytext);
    return tTEXT;
}
%%
