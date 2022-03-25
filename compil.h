#ifndef COMPIL_H
#define COMPIL_H

#include "symbol_table.h"
#include "function_table.h"
#include "type.h"

struct compil {
    struct symbol_table * symbol_table;
    struct function_table * function_table;
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
void compil_enter_scope();
void compil_exit_scope();

void compil_expr_push(char * name);

void compil_print();
#endif //COMPIL_H
