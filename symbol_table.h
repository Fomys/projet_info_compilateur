#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "instruction_array.h"

enum symbol_kind {
    SYMBOL_UNKNOWN,
    SYMBOL_VAR,
    SYMBOL_TYPE,
    SYMBOL_FUNCTION
};

struct symbol_var {
    struct symbol* type;
};

struct symbol_function {
    struct arguments* arguments;
    struct AST_node* ret;
    struct AST_node* body;
    struct instruction_array* assembled;
};

struct symbol {
    enum symbol_kind kind;
    unsigned int usage_count;
    char* name;
    union {
        struct symbol_var symbol_var;
        struct symbol_function symbol_function;
    };
};

struct symbol_table {
    struct symbol_table* parent;
    struct symbol** symbols;
    unsigned int symbols_count;
};

struct arguments {
    struct symbol** arguments;
    unsigned int arguments_count;
};

void symbol_table_init();

struct symbol_table* symbol_table_alloc();
void symbol_table_free(struct symbol_table*);

struct symbol* symbol_alloc();
void symbol_free(struct symbol*);

struct arguments* arguments_alloc();
void arguments_free(struct arguments*);
void add_argument(struct arguments*, struct symbol*);

void symbol_print(struct symbol*);
struct symbol* symbol_table_get(struct symbol_table*, char*);

void symbol_set_kind(struct symbol*, enum symbol_kind);

void symbol_table_print(struct symbol_table*);

struct symbol* symbol_table_current_get_or_insert(char*);
void symbol_table_current_print();
void symbol_table_current_enter_scope();
void symbol_set_arguments(struct symbol*, struct arguments*);
void arguments_print(struct arguments*);
void symbol_set_ast(struct symbol*, struct AST_node*);
void symbol_table_current_exit_scope();
struct symbol* symbol_table_create_anonymous(struct symbol_table*);
struct symbol_table* symbol_table_current_get();
#endif //SYMBOL_TABLE_H
