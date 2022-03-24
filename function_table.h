#ifndef FUNCTION_TABLE_h
#define FUNCTION_TABLE_h

#include "symbol_table.h"

struct function {
    enum type type;
    char name[128];
    int param_count;
    enum type params[128];    
};

struct function_table {
    int function_count;
    struct function functions[128];
};

void initialize_function_table ();

void add_function(enum type type, char *name);

void add_param(enum type type);

#endif // FUNCTION_TABLE_h