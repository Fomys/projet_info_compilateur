%{
#include <stdlib.h>
#include <stdio.h>
#include "consts.h"
#include "symbol_table.h"
#include "function_table.h"
void yyerror(char *s);
%}

%union {int nb; char text[128];}
%token tIF tELSE tWHILE tOPAR tCPAR tOCUR tCCUR tRET tPV tASSIGN tADD tMUL tGT tLT tMIN tAND tOR tEQ tINT tCONST tVOID tCOM tDIV tPRINT
%token <text> tTEXT
%token <nb> tNB

%left tADD tMIN
%left tMUL tDIV

%start StartCompilo
%%
StartCompilo: Compilo {initialize_symbol_table(); initialize_function_table();};

Compilo	:	Fonction
	| Declaration tPV
	| tCONST Declaration tPV
	| Compilo tCONST Declaration tPV
	| Compilo Declaration tPV
	| Compilo Fonction;

Declaration	:	tINT tTEXT {add_symbol(TYPE_INT, $2);};
DeclarationFonctionInt : tINT tTEXT {add_function(TYPE_INT, $2);};
DeclarationFonctionVoid : tVOID tTEXT {add_function(TYPE_VOID, $2);};
DeclarationParam	:	tINT tTEXT {add_symbol(TYPE_INT, $2); add_param(TYPE_INT);};

EnterBlock : {enter_scope();};
EndBlock : {exit_scope();};


Rien : ;

Lignes	: Rien
	| Lignes Ligne;

Ligne	:	tPV
	|	Declaration tPV
	|	Declaration tASSIGN Expression tPV;
	|	tTEXT tASSIGN Expression tPV
	|	AppelFonction
	|	BlocIf
	|	BlocWhile
    |   tPRINT tOPAR Expression tCPAR tPV {};

Fonction	: FonctionInt | FonctionVoid;
FonctionInt	:	DeclarationFonctionInt tOPAR EnterBlock Parametres tCPAR tOCUR
					Lignes
					tRET Expression tPV
				tCCUR EndBlock ;

FonctionVoid	:	DeclarationFonctionVoid tOPAR EnterBlock Parametres tCPAR tOCUR
					    Lignes
				    tCCUR EndBlock;

Parametres	: Rien
	| tVOID
	| DeclarationParam
	| Parametres tCOM DeclarationParam;

Arguments	: Rien
	|	Expression
	|	Arguments tCOM Expression;

AppelFonction	: tTEXT tOPAR Arguments tCPAR;

Expression	: tNB
	| tTEXT
	| AppelFonction
	| Expression Operateur Expression
	| tOPAR Expression tCPAR ;

Operateur	:	tADD | tMUL | tMIN | tDIV | tEQ | tOR | tAND | tGT | tLT;

BlocIf	:	tIF tOPAR Expression tCPAR tOCUR EnterBlock Lignes tCCUR EndBlock
		|	tIF tOPAR Expression tCPAR tOCUR EnterBlock Lignes tCCUR EndBlock tELSE tOCUR EnterBlock Lignes tCCUR EndBlock;

BlocWhile	:	tWHILE tOPAR Expression tCPAR tOCUR EnterBlock Lignes tCCUR EndBlock;
%%
