#include "compil.h"

struct compil compil;

void compil_initialize() {
    compil.symbol_table = symbol_table_alloc();
    compil.function_table = function_table_alloc();
}

void compil_add_symbol(enum type type, char * name) {
    struct symbol * symbol = symbol_alloc();
    symbol->type = type;
    symbol_set_name(symbol, name);
    symbol_table_push_symbol(compil.symbol_table, symbol);
}

void compil_add_function(enum type type, char * name) {
    struct function * function = function_alloc();
    function_set_name(function, name);
    function->type = type;
    function_table_push_function(compil.function_table, function);
}

void compil_add_param(enum type type, char * name) {
    // Ajout du symbole dans la table
    struct symbol * symbol = symbol_alloc();
    symbol->type = type;
    symbol_set_name(symbol, name);
    symbol_table_push_symbol(compil.symbol_table, symbol);
    // Ajout de la fonction
    struct function * function = function_table_get_last_function(compil.function_table);
    function_add_param(function, type);
    compil_print();
}

void compil_enter_scope() {
    symbol_table_enter_scope(compil.symbol_table);
}

void compil_exit_scope() {
    symbol_table_exit_scope(compil.symbol_table);
    compil_print();
}

void compil_print() {
    symbol_table_print(compil.symbol_table);
    function_table_print(compil.function_table);
}

//void compil_assign(char * name, );