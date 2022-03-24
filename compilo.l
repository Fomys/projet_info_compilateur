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
"&&"	{return tAND;}
"||"	{return tOR;}
"=="	{return tEQ;}
"int"	{return tINT;}
"const"	{return tCONST;}
"//.*$"	{}
"void"	{return tVOID;}
"/"	{return tDIV;}
-?[0-9]+	{
    //yylval.nb = atoi(yytext[0]);
    return tNB;
}
[a-z]+	{
    //yylval.text = yytext[0];
    return tTEXT;
}
%%
