%{
#include <stdlib.h>
#include <stdio.h>
void yyerror(char *s);
%}
%token tIF tELSE tWHILE tOPAR tCPAR tOCUR tCCUR tRET tPV tASSIGN tADD tMUL tGT tLT tMIN tAND tOR tEQ tINT tCONST tVOID tCOM tDIV
%token tTEXT
%token tNB
%start Compilo
%%
Compilo	:	Fonction
	| Declaration tPV
	| tCONST Declaration tPV
	| Compilo tCONST Declaration tPV
	| Compilo Declaration tPV
	| Compilo Fonction;

Fonction	: FonctionInt | FonctionVoid;
FonctionInt	:	tINT tTEXT tOPAR Parametres tCPAR tOCUR
					Lignes
					tRET Expression tPV
				tCCUR;

FonctionVoid	:	tVOID tTEXT tOPAR Parametres tCPAR tOCUR
					Lignes
				tCCUR;

Parametres	:
	| tVOID
	| Declaration
	| Parametres tCOM Declaration;

Declaration	:	tINT tTEXT;


Lignes	:
	| Lignes Ligne;

Ligne	:	tPV
	|	Declaration tPV
	|	Declaration tASSIGN Expression tPV;
	|	tTEXT tASSIGN Expression tPV
	|	AppelFonction
	|	BlocIf
	|	BlocWhile;

Arguments	:
	|	Expression
	|	Arguments tCOM Expression;

AppelFonction	:
	tTEXT tOPAR Arguments tCPAR;

Expression	: tNB
	| tTEXT
	| AppelFonction
	| Expression Operateur Expression
	| tOPAR Expression tCPAR ;

Operateur	:	tADD | tMUL | tMIN | tDIV | tEQ | tOR | tAND | tGT | tLT;

BlocIf	:	tIF tOPAR Expression tCPAR tOCUR Lignes tCCUR
		|	tIF tOPAR Expression tCPAR tOCUR Lignes tCCUR tELSE tOCUR Lignes tCCUR;

BlocWhile	:	tWHILE tOPAR Expression tCPAR tOCUR Lignes tCCUR;
%%
