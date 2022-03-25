#ifndef FUNCTION_TABLE_h
#define FUNCTION_TABLE_h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"

struct function {
    enum type type;
    char * name;
    int param_count;
    enum type * params;
};

struct function_table {
    int function_count;
    struct function ** functions;
};

/*
 * Alloue la mémoire pour une fonction
 */
struct function * function_alloc();
/*
 * Libère la mémoire associé à une fonction
 */
void function_free(struct function * function);
/*
 * Défini le nom de la fonction
 *
 * char * name: copié dans la structure, c'est à l'appellant de libérer la mémoire
 */
void function_set_name(struct function * function, char * name);
/*
 * Ajoute un paramètre à une fonction
 */
void function_add_param(struct function * function, enum type type);
void function_print(struct function * function);

/*
 * Alloue la mémoire associé à la table des fonctions
 */
struct function_table * function_table_alloc();
/*
 * Libère la mémoire associé à la table des fonctions
 */
void function_table_free(struct function_table * table);
/*
 * Ajoute une fonction à la table
 *
 * struct function * function: prend la propriété de ce pointeur, celui-ci ne doit pas être libéré manuellement
 */
void function_table_push_function(struct function_table * table, struct function * function);
/*
 * Renvoie un pointeur vers la dernière fonction, le pointeur renvoyé est encore la propriété de la table, il ne
 * doit pas être libéré manuellement.
 */
struct function * function_table_get_last_function(struct function_table * table);
void function_table_print(struct function_table * table);

#endif // FUNCTION_TABLE_h