%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap

%%
"if"	{printf("if")}
"while"	{printf("while");}
"for"	{printf("for");}
"("	{printf("(");}
")"	{printf(")");}
"{"	{printf("{");}
"}"	{printf("}");}
"return"	{printf("return");}
";"	{printf(";");}
