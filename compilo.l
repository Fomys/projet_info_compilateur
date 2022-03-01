%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap

%%
"if"	{return tIF;}
"while"	{return tWHILE;}
"("	{return tOPAR;}
")"	{return tCPAR;}
"{"	{return tOCUR;}
"}"	{return tCCUR;}
"["	{return tOBRA;}
"]"	{return tCBRA;}
"return"	{return tRET;}
";"	{return tPV;}
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
-?[0-9]+	{}
[a-z]+	{}
