%{
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "symbol_table.h"
#include "assembler.h"

void yyerror(char *s);
extern FILE *yyout;

#define YYLTYPE (struct location);
%}

%union {
    char* identifier;
    struct symbol* symbol;
    struct arguments* arguments;
    struct AST_node* node;
    struct call_parameters* parameters;
}
%token tIF tELSE tWHILE tOPAR tCPAR tOCUR tCCUR tRET tPV tASSIGN tADD tMUL tGT tLT tMIN tEQ tFN tCOM tDIV tPRINT
%token <identifier> tTEXT
%token <node> tNB

%type <node> Lines
%type <symbol> Symbol
%type <node> Assignation
%type <node> Declaration
%type <node> Expression
%type <arguments> Arguments
%type <node> AppelFonction
%type <node> If
%type <parameters> Parameters
%type <node> Else
%type <node> Return

%left tEQ
%left tLT tGT
%left tADD tMIN
%left tMUL tDIV


%start StartCompilo
%%
StartCompilo: {
    symbol_table_init();
} Lines {
    AST_set_symbol_table($2, symbol_table_current_get());
    AST_node_print($2);
    struct instruction_array* instructions = assemble($2);
    instruction_array_print_raw(instructions, yyout);
    instruction_array_print(instructions);
};

Lines	:
    { $$ = AST_node_body_alloc(); } |
    Lines Assignation tPV { AST_node_body_add_child($1, $2); $$ = $1;} |
    Lines Declaration tPV { AST_node_body_add_child($1, $2); $$ = $1;} |
    Lines Expression tPV { AST_node_body_add_child($1, $2); $$ = $1; } |
    Lines Function { $$ = $1; } |
    Lines If {$$ = $1; AST_node_body_add_child($$, $2);} |
    Lines Return { AST_node_body_add_child($$, $2), $$ = $1;};
Return : tRET Expression tPV {$$ = AST_node_return_from($2); };
Declaration :
    Symbol {
        symbol_set_kind($1, SYMBOL_VAR);
        $$ = AST_node_declaration_from($1);
    };

Assignation	:
    Symbol tASSIGN Expression { $$ = AST_node_assignement_from($1, $3); };

Expression :
    tNB { $$ = $1; } |
    Symbol { $$ = AST_node_symbol_from($1); } |
    Expression tADD Expression { $$ = AST_node_operation_from(OP_ADD, $1, $3);} |
    Expression tMUL Expression { $$ = AST_node_operation_from(OP_MUL, $1, $3); } |
    Expression tDIV Expression { $$ = AST_node_operation_from(OP_DIV, $1, $3); } |
    Expression tMIN Expression { $$ = AST_node_operation_from(OP_SUB, $1, $3); } |
    Expression tEQ Expression { $$ = AST_node_operation_from(OP_EQ, $1, $3); } |
    Expression tGT Expression { $$ = AST_node_operation_from(OP_GT, $1, $3); } |
    Expression tLT Expression { $$ = AST_node_operation_from(OP_LT, $1, $3); } |
    tOPAR Expression tCPAR { $$ = $2; } |
    AppelFonction { $$ = $1; };

AppelFonction : Symbol tOPAR Parameters tCPAR  {$$ = AST_node_call_from($1, $3);};

Parameters :
    { $$ = call_parameters_alloc();} |
    Expression { $$ = call_parameters_alloc(); call_parameters_add($$, $1); } |
    Parameters tCOM Expression {$$ = $1; call_parameters_add($$, $3); };

Symbol : tTEXT { $$ = symbol_table_current_get_or_insert($1); };

Function : tFN Symbol tOPAR {
    symbol_table_current_enter_scope();
} Arguments tCPAR tOCUR Lines tCCUR {
    symbol_set_arguments($2, $5);
    symbol_set_kind($2, SYMBOL_FUNCTION);
    AST_set_symbol_table($8, symbol_table_current_get());
    symbol_set_ast($2, $8);
    symbol_table_current_exit_scope();
};


Arguments :
    { $$ = arguments_alloc(); } |
    Symbol { $$ = arguments_alloc(); add_argument($$, $1); } |
    Arguments tCOM Symbol { $$ = $1; add_argument($$, $3); };

If :
    tIF Expression EnterScope Lines {
        AST_set_symbol_table($4, symbol_table_current_get());
    } ExitScope Else {
        $$ = AST_node_if_else_from($2, $4, $7);
    };
Else :
    { $$ = AST_node_body_alloc(); symbol_table_current_enter_scope(); AST_set_symbol_table($$, symbol_table_current_get()); symbol_table_current_exit_scope();} |
    tELSE EnterScope Lines {
        AST_set_symbol_table($3, symbol_table_current_get());
    } ExitScope {
        $$ = $3;
    };

EnterScope : tOCUR {symbol_table_current_enter_scope();};

ExitScope : tCCUR {symbol_table_current_exit_scope();};

%%
