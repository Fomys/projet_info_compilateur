#include "function_table.h"

struct function * function_alloc() {
    struct function * function = malloc(sizeof(struct function));
    if (function == NULL) {
        perror("Failed to allocate memory for function.");
        exit(EXIT_FAILURE);
    }
    function->param_count = 0;
    function->type = TYPE_VOID;
    function->name = malloc(0);
    function->params = malloc(0);
    return function;
}
void function_free(struct function * function) {
    free(function->params);
    free(function->name);
    free(function);
}
void function_set_name(struct function * function, char * name) {
    function->name = malloc(strlen(name) * sizeof(char));
    if (function->name == NULL) {
        perror("Failed to allocate memory for symbol name.");
        exit(EXIT_FAILURE);
    }
    strcpy(function->name, name);
}
void function_add_param(struct function * function, enum type type) {
    function->param_count += 1;
    function->params = reallocarray(function->params, function->param_count, sizeof(enum type));
    if(function->params == NULL) {
        perror("Failed to allocate memory for function parameter.");
        exit(EXIT_FAILURE);
    }
    function->params[function->param_count - 1] = type;
}
void function_print(struct function * function) {
    type_print(function->type);
    printf(" %s(", function->name);
    for(unsigned int i = 0; i < function->param_count; i++) {
        type_print(function->params[i]);
        printf(", ");
    }
    printf(")\n");
}

struct function_table * function_table_alloc() {
    struct function_table * table = malloc(sizeof(struct function_table));
    table->function_count = 0;
    table->functions = malloc(0);
    return table;
}
void function_table_free(struct function_table * table) {
    for(unsigned int i = 0; i < table->function_count; i++) {
        function_free(table->functions[i]);
    }
    free(table);
}
void function_table_push_function(struct function_table * table, struct function * function) {
    table->function_count += 1;
    table->functions = reallocarray(table->functions, table->function_count, sizeof(struct function *));
    if(table->functions == NULL) {
        perror("Failed to allocate memory for new function.");
        exit(EXIT_FAILURE);
    }
    table->functions[table->function_count - 1] = function;

}
struct function * function_table_get_last_function(struct function_table * table) {
    return table->functions[table->function_count-1];
}
void function_table_print(struct function_table * table) {
    printf("Function table:\n");
    for(unsigned int i = 0; i < table->function_count; i++) {
        function_print(table->functions[i]);
    }
    printf("\n");
}