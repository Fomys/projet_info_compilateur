#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"

struct symbol {
    enum type type;
    unsigned int depth;
    char * name;
};

struct symbol_table {
    unsigned int symbol_count;
    unsigned int current_depth;
    struct symbol ** symbols;
};

/*
 * Alloue la mémoire pour un symbole
 */
struct symbol * symbol_alloc();
/*
 * Libère la mémoire pour un symbole
 */
void symbol_free(struct symbol * symbol);
/*
 * Défini le nom du symbole
 *
 * char * name: copié dans la structure, c'est à l'appellant de libérer la mémoire
 */
void symbol_set_name(struct symbol * symbol, char * name);
void symbol_print(struct symbol * symbol);

/*
 * Alloue la mémoire associé à la table des symbole
 */
struct symbol_table * symbol_table_alloc ();
/*
 * Libère la mémoire associé à la table des symboles
 */
void symbol_table_free(struct symbol_table * table);
/*
 * Ajoute un symbole à la table des symboles
 *
 * struct symbol * symbol: prend la propriété de ce pointeur, celui-ci ne doit pas être libéré manuellement
 */
void symbol_table_push_symbol(struct symbol_table * table, struct symbol * symbol);
/*
 * Les prochains symboles ajoutés seront dans un scope plus restreint.
 */
void symbol_table_enter_scope(struct symbol_table * table);
/*
 * On sort du scope actuel, les variables associées sont supprimées
 */
void symbol_table_exit_scope(struct symbol_table * table);
/*
 * Récupération de l'adresse associé à une variable relativement au scope actuel
 *
 * Une valeur positive signifie que c'est une variable locale, une adresse négative
 */
unsigned int symbol_table_get_address(struct symbol_table * table, char * name);
void symbol_table_print(struct symbol_table * table);
#endif // SYMBOL_TABLE_H