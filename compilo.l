%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
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
"int"	{return tINT;}
"const"	{return tCONST;}
"//.*$"	{}
"void"	{return tVOID;}
"print" {return tPRINT;}
"/"	{return tDIV;}
-?[0-9]+	{
    yylval.nb = atoi(yytext);
    return tNB;
}
[a-z]+	{
    strncpy(yylval.text, yytext, 128);
    return tTEXT;
}
%%
