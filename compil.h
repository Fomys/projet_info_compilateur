#ifndef COMPIL_H
#define COMPIL_H

#include "symbol_table.h"
#include "function_table.h"
#include "type.h"
#include "program.h"

struct compil {
    struct symbol_table * symbol_table;
    struct function_table * function_table;
    int work_pointer;
};

/*
 * Initialise le compilateur, alloue la mémoire
 */
void compil_initialize();
/*
 * Ajoute un symbole dans le scope courant
 */
void compil_add_symbol(enum type type, char * name);
/*
 * Ajoute une fonction
 */
void compil_add_function(enum type type, char * name);
/*
 * Ajoute un paramètre à la dernière fonction ajoutée
 */
void compil_add_param(enum type type, char * name);
/*
 * Enregistre le changement de scope
 */
void compil_enter_scope();
/*
 * Enregistre le changement de scope et supprime les variables inutiles
 */
void compil_exit_scope();

/*
 * Enregistre un nombre dans l'expression
 */
int compil_expr_push_nb(int nb);
/*
 * Enregistre un text dans l'expression
 */
int compil_expr_push_var(char * name);

/*
 * Effectue une addition dans la pile
 */
int compil_add(int a, int b);
/*
 * Effectue une soustraction dans la pile
 */
int compil_min(int a, int b);
/*
 * Effectue une multiplication dans la pile
 */
int compil_mul(int a, int b);
/*
 * Effectue une division dans la pile
 */
int compil_div(int a, int b);
/*
 * Effectue une equivalence dans la pile
 */
int compil_eq(int a, int b);
/*
 * Effectue une comparaison > dans la pile
 */
int compil_gt(int a, int b);
/*
 * Effectue une comparaison < dans la pile
 */
int compil_lt(int a, int b);

void compil_assign(char * name, int addr);

/*
 * Affiche le compilateur
 */
void compil_print();

void compil_print_asm();

void compil_start_if(int addr);
void compil_patch_if(int pc);
int compil_get_pc();
#endif //COMPIL_H
