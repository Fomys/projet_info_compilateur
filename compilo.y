%{
#include <stdlib.h>
#include <stdio.h>
#include "compil.h"

void yyerror(char *s);
%}

%union {int nb; char text[128];}
%token tIF tELSE tWHILE tOPAR tCPAR tOCUR tCCUR tRET tPV tASSIGN tADD tMUL tGT tLT tMIN tEQ tINT tCONST tVOID tCOM tDIV tPRINT
%token <text> tTEXT
%token <nb> tNB

%type <text> Declaration
%type <nb> Expression
%type <nb> Operation
%type <nb> FinIf

%left tEQ
%left tLT tGT
%left tADD tMIN
%left tMUL tDIV


%start StartCompilo
%%
StartCompilo: InitializeCompilo Compilo {compil_print_asm();};
InitializeCompilo : {compil_initialize();};

Compilo	: 	Rien
		| 	Compilo Declaration tPV
		| 	Compilo Declaration tASSIGN Expression tPV {compil_assign($2, $4);}
		| 	Compilo Fonction;

Declaration	: tINT tTEXT {compil_add_symbol(TYPE_INT, $2); strcpy(&$$,$2);}
DeclarationFonctionInt : tINT tTEXT {compil_add_function(TYPE_INT, $2);};
DeclarationFonctionVoid : tVOID tTEXT {compil_add_function(TYPE_VOID, $2);};
DeclarationParam : tINT tTEXT {compil_add_param(TYPE_INT, $2);};

EnterBlock : {compil_enter_scope();};
EndBlock : {compil_exit_scope();};

Rien : ;

Lignes	: 	Rien
		| 	Lignes Ligne;

Ligne	: 	tPV
		| 	Declaration tPV
		| 	Declaration tASSIGN Expression tPV {compil_assign($1, $3);}
		| 	tTEXT tASSIGN Expression tPV {compil_assign($1, $3);}
		| 	AppelFonction
		| 	BlocIf
		| 	BlocWhile
   		| 	tPRINT tOPAR Expression tCPAR tPV {printf("coucou\n");compil_print($3);};

Fonction	: 	FonctionInt | FonctionVoid;
FonctionInt	:	DeclarationFonctionInt tOPAR EnterBlock Parametres tCPAR tOCUR
					Lignes
					tRET Expression tPV
				tCCUR EndBlock ;

FonctionVoid	:	DeclarationFonctionVoid tOPAR EnterBlock Parametres tCPAR tOCUR
					    Lignes
				    tCCUR EndBlock;

Parametres	: 	Rien
			| 	tVOID
			| 	DeclarationParam
			|	 Parametres tCOM DeclarationParam;

Arguments	: 	Rien
			| 	Expression
			| 	Arguments tCOM Expression;

AppelFonction	: 	tTEXT tOPAR Arguments tCPAR;

Expression	: 	tNB {$$ = compil_expr_push_nb($1);}
			| 	tTEXT {$$ = compil_expr_push_var($1);}
			| 	AppelFonction
			| 	Operation
			| 	tOPAR Expression tCPAR {$$ = $2;};

Operation	: 	Expression tADD Expression {$$ = compil_add($1,$3);}
			| 	Expression tMIN Expression {$$ = compil_min($1,$3);}
			| 	Expression tMUL Expression {$$ = compil_mul($1,$3);}
			| 	Expression tDIV Expression {$$= compil_div($1,$3);}
			| 	Expression tEQ Expression {$$= compil_eq($1,$3);}
			| 	Expression tGT Expression {$$= compil_gt($1,$3);}
			| 	Expression tLT Expression {$$= compil_lt($1,$3);};

BlocIf	: 	tIF tOPAR Expression tCPAR  {$<nb>1 = compil_get_pc();compil_start_if($3);} tOCUR EnterBlock Lignes tCCUR EndBlock FinIf {compil_patch_if($<nb>1,$11);}  ;

FinIf 	:	Rien {$$=compil_get_pc();}
		|	tELSE  {$<nb>1 = compil_get_pc(); compil_start_else(); $<nb>$=compil_get_pc();} tOCUR EnterBlock Lignes tCCUR EndBlock {compil_patch_else($<nb>1); $$=$<nb>2;};

BlocWhile	: 	tWHILE {$<nb>1 = compil_get_pc();} tOPAR Expression tCPAR {compil_start_while($4);} tOCUR EnterBlock Lignes tCCUR EndBlock {compil_patch_while($<nb>1);};
%%
