#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum type {
    TYPE_INT,
    TYPE_VOID
};

typedef struct symbol {
    enum type type;
    int depth;
    char name[128];
} symbol;

typedef struct symbol_table {
    int symbol_count;
    struct symbol symbols[128];
} symbol_table;


void initialize_symbol_table ();

void add_symbol(enum type type, char *name);

void enter_scope();

void exit_scope();


#endif // SYMBOL_TABLE_H