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
"["	{return tOBRA;}
"]"	{return tCBRA;}
"return"	{return tRET;}
";"	{return tPV;}
","	{return tCOM;}
[ \t\n]	{return tSPA;}
"="	{return tASSIGN;}
"+"	{return tADD;}
"*"	{return tMUL;}
">"	{return tGT;}
"<"	{return tLT;}
"-"	{return tMIN;}
"&&"	{return tAND;}
"||"	{return tOR;}
"=="	{return tEQ;}
"int"	{return tINT;}
"const"	{return tCONST;}
"//.*$"	{return tCOMMENT;}
"void"	{return tVOID;}
-?[0-9]+	{
    yylval.nb = atoi(yytext[0]);
    return tNB;
}
[a-z]+	{
    yylval.text = yytext[0];
    return tTEXT;
}
%%
