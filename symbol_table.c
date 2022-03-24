#include "symbol_table.h"
#include <string.h>

struct symbol_table global_symbol_table;

int current_depht = 0;

void print_table() {
    for(int i = 0; i < global_symbol_table.symbol_count; i++) {
        for (int j = 0; j < global_symbol_table.symbols[i].depth; j++) {
            printf(" ");
        }
        printf("%d - %s\n", i, global_symbol_table.symbols[i].name);
    }
    printf("\n\n");
}

void initialize_symbol_table () {
    //void * symbols = malloc(0);
    global_symbol_table.symbol_count = 0;
    //global_symbol_table.symbols = symbols;
}

void add_symbol(enum type type, char *name) {
    symbol symbol = {
        .type = type, 
        .depth = current_depht,
    };
    strcpy(&symbol.name, name);
    global_symbol_table.symbols[global_symbol_table.symbol_count] = symbol;
    global_symbol_table.symbol_count += 1;
}

void enter_scope() {
    current_depht++;
}

void exit_scope() {
    current_depht--;
    for(int i = global_symbol_table.symbol_count; i >= 0; i--) {
        if (global_symbol_table.symbols[i].depth > current_depht) {
            global_symbol_table.symbol_count --;
        }
    }
}

