#include "symbol_table.h"

struct symbol * symbol_alloc() {
    struct symbol * symbol = malloc(sizeof(struct symbol));
    if (symbol == NULL) {
        perror("Failed to allocate memory for symbol.");
        exit(EXIT_FAILURE);
    }
    symbol->depth = 0;
    symbol->name = malloc(0);
    symbol->type = TYPE_VOID;
    return symbol;
}
void symbol_free(struct symbol * symbol) {
    free(symbol->name);
    free(symbol);
}
void symbol_set_name(struct symbol * symbol, char * name) {
    symbol->name = malloc(strlen(name) * sizeof(char));
    if (symbol->name == NULL) {
        perror("Failed to allocate memory for symbol name.");
        exit(EXIT_FAILURE);
    }
    strcpy(symbol->name, name);
}
void symbol_print(struct symbol * symbol) {
    for(unsigned int i = 0; i < symbol->depth; i++) {
        printf(" ");
    }
    printf("%s\n", symbol->name);
}

struct symbol_table * symbol_table_alloc () {
    struct symbol_table * table = malloc(sizeof(struct symbol_table));
    if (table == NULL) {
        perror("Failed to allocate memory for symbol table.");
        exit(EXIT_FAILURE);
    }
    table->symbol_count = 0;
    table->symbols = malloc(0);
    return table;
}
void symbol_table_free(struct symbol_table * table) {
    for(int i = 0; i < table->symbol_count; i++) {
        symbol_free(table->symbols[i]);
    }
    free(table->symbols);
    free(table);
}
void symbol_table_push_symbol(struct symbol_table * table, struct symbol * symbol) {
    table->symbol_count += 1;
    table->symbols = reallocarray(table->symbols, table->symbol_count, sizeof(struct symbol *));
    if(table->symbols == NULL) {
        perror("Failed to allocate memory for symbol table.");
        exit(EXIT_FAILURE);
    }
    table->symbols[table->symbol_count - 1] = symbol;
    symbol->depth = table->current_depth;
}
void symbol_table_enter_scope(struct symbol_table * table) {
    table->current_depth += 1;
}
void symbol_table_exit_scope(struct symbol_table * table) {
    table->current_depth -= 1;
    while(table->symbol_count > 0 && table->symbols[table->symbol_count - 1]->depth > table->current_depth) {
        symbol_free(table->symbols[table->symbol_count - 1]);
        table->symbol_count--;
    }
}
unsigned int symbol_table_get_address(struct symbol_table * table, char * name) {
    for(unsigned int i = 0; i < table->symbol_count; i++) {
        if(strcmp(name, table->symbols[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}
void symbol_table_print(struct symbol_table * table) {
    printf("Symbol table:\n");
    for(unsigned int i = 0; i < table->symbol_count; i++) {
        symbol_print(table->symbols[i]);
    }
    printf("\n");
}