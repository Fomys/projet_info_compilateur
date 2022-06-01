#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "ast.h"
#include "assembler.h"

struct symbol_table* symbol_table_current;

void symbol_table_init() {
    symbol_table_current = symbol_table_alloc();
}

struct symbol_table * symbol_table_alloc() {
    struct symbol_table* symbol_table = malloc(sizeof(symbol_table));
    if(symbol_table == NULL) {
        perror("Failed to malloc for symboltable.\n");
        exit(errno);
    }
    symbol_table->symbols = malloc(0);
    symbol_table->symbols_count = 0;
    symbol_table->parent = NULL;
    return symbol_table;
}

void symbol_table_free(struct symbol_table* symbol_table) {
    for(unsigned int i = 0; i < symbol_table->symbols_count; i++) {
        symbol_free(symbol_table->symbols[i]);
    }
    free(symbol_table->symbols);
    free(symbol_table);
}

struct symbol* symbol_alloc() {
    struct symbol* symbol = malloc(sizeof(struct symbol));
    if(symbol == NULL) {
        perror("Failed to malloc for symbol.\n");
        exit(errno);
    }
    symbol->kind = SYMBOL_UNKNOWN;
    symbol->name = malloc(0);
    symbol->usage_count = 0;
    return symbol;
}

void symbol_free(struct symbol* symbol) {
    switch(symbol->kind) {
        case SYMBOL_UNKNOWN:
        case SYMBOL_VAR:
        case SYMBOL_TYPE:
            break;
        case SYMBOL_FUNCTION:
            AST_node_free(symbol->symbol_function.body);
            instruction_array_free(symbol->symbol_function.assembled);
            break;
    }
    free(symbol->name);
    free(symbol);
}

void symbol_print(struct symbol* symbol) {
    switch(symbol->kind) {
        case SYMBOL_UNKNOWN:
            printf("UNKNOWN(");
            break;
        case SYMBOL_TYPE:
            printf("TYPE(");
            break;
        case SYMBOL_VAR:
            printf("VAR(");
            break;
        case SYMBOL_FUNCTION:
            printf("FUNCTION(");
            arguments_print(symbol->symbol_function.arguments);
            printf(" ");
            break;
    }
    if(symbol->name == NULL) {
        printf("%p)", symbol);
    } else {
        printf("%s)", symbol->name);
    }
}

void symbol_table_print(struct symbol_table* symbol_table) {
    if(symbol_table->parent != NULL) {
        symbol_table_print(symbol_table->parent);
    }
    printf("Symbol table: \n");
    for(unsigned int i = 0; i < symbol_table->symbols_count; i++) {
        printf(" ");
        symbol_print(symbol_table->symbols[i]);
        printf("\n");
    }
}

struct symbol* symbol_table_get(struct symbol_table* symbol_table, char* symbol) {
    for(unsigned int i = 0; i < symbol_table->symbols_count; i++) {
        if(strcmp(symbol_table->symbols[i]->name, symbol) == 0) {
            return symbol_table->symbols[i];
        }
    }
    if(symbol_table->parent != NULL) {
        return symbol_table_get(symbol_table->parent, symbol);
    } else {
        return NULL;
    }
}

struct symbol* symbol_table_get_or_insert(struct symbol_table* symbol_table, char* name) {
    struct symbol* symbol = symbol_table_get(symbol_table, name);
    if(symbol == NULL) {
        symbol = symbol_alloc();
        symbol_table->symbols_count++;
        symbol_table->symbols = reallocarray(symbol_table_current->symbols, symbol_table_current->symbols_count, sizeof(struct symbol*));
        symbol_table->symbols[symbol_table_current->symbols_count - 1] = symbol;
        symbol->name = malloc(strlen(name));
        strcpy(symbol->name, name);
    }
    return symbol;
}

struct symbol* symbol_table_current_get_or_insert(char* name) {
    return symbol_table_get_or_insert(symbol_table_current, name);
}


void symbol_set_kind(struct symbol* symbol, enum symbol_kind kind) {
    if(symbol->kind != SYMBOL_UNKNOWN) {
        fprintf(stderr, "The symbol %s is already defined:", symbol->name);
        exit(1);
    }
    symbol->kind = kind;
}


void symbol_table_current_print() {
    symbol_table_print(symbol_table_current);
}

struct arguments* arguments_alloc() {
    struct arguments* arguments = malloc(sizeof(arguments));
    if (arguments == NULL) {
        perror("Failed to malloc for arguments.\n");
        exit(errno);
    }
    arguments->arguments = malloc(0);
    arguments->arguments_count = 0;
    return arguments;
}

void arguments_free(struct arguments* arguments) {
    free(arguments->arguments);
    free(arguments);
}

void add_argument(struct arguments* arguments, struct symbol* symbol) {
    arguments->arguments_count++;
    arguments->arguments = reallocarray(arguments->arguments, arguments->arguments_count, sizeof(struct symbol*));
    arguments->arguments[arguments->arguments_count - 1] = symbol;
    symbol_set_kind(arguments->arguments[arguments->arguments_count - 1], SYMBOL_VAR);
}

void symbol_table_current_enter_scope() {
    struct symbol_table* new_table = symbol_table_alloc();
    new_table->parent = symbol_table_current;
    symbol_table_current = new_table;
}

void symbol_set_arguments(struct symbol* symbol, struct arguments* arguments) {
    symbol->symbol_function.arguments = arguments;
}

void symbol_set_ret(struct symbol* symbol, struct AST_node* node) {
    symbol->symbol_function.ret = node;
}

void arguments_print(struct arguments* arguments) {
    for(unsigned int i = 0; i < arguments->arguments_count; i ++) {
        if(i != 0) {
            printf(", ");
        }
        symbol_print(arguments->arguments[i]);
    }
}

void symbol_set_ast(struct symbol* symbol, struct AST_node* body) {
    symbol->symbol_function.body = body;
}

void symbol_table_current_exit_scope() {
    symbol_table_current = symbol_table_current->parent;
}

struct symbol* symbol_table_create_anonymous(struct symbol_table* symbol_table) {
    struct symbol* symbol = symbol_alloc();
    symbol_table->symbols_count++;
    symbol_table->symbols = reallocarray(symbol_table_current->symbols, symbol_table_current->symbols_count, sizeof(struct symbol*));
    symbol_table->symbols[symbol_table_current->symbols_count - 1] = symbol;
    symbol->name = NULL;
    return symbol;
}

struct symbol_table* symbol_table_current_get() {
    return symbol_table_current;
}