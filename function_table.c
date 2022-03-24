 #include "function_table.h"

struct function_table global_function_table;

void initialize_function_table () {
    global_function_table.function_count = 0;
}

void print_functions() {
    for(int i = 0; i < global_function_table.function_count; i++) {
        printf("%d - %s(", global_function_table.functions[i].param_count, global_function_table.functions[i].name);
        for(int j = 0; j < global_function_table.functions[i].param_count; j++) {
            switch (global_function_table.functions[i].params[i]) {
                case TYPE_VOID:
                    printf("void");
                    break;
                case TYPE_INT:
                    printf("int");
                    break;
            }
            printf(",");
        }
        printf(")\n");
    }
    printf("\n\n");
}

void add_function(enum type type, char *name) {
    struct function function = {
        .type = type, 
    };
    strcpy(&function.name, name);
    global_function_table.functions[global_function_table.function_count] = function;
    global_function_table.function_count += 1;
    print_functions();
}

void add_param(enum type type) {
    //global_function_table.functions[global_function_table.function_count].param_count = global_function_table.functions[global_function_table.function_count-1].param_count +1;
    int param_count = global_function_table.functions[global_function_table.function_count-1].param_count++;
    global_function_table.functions[global_function_table.function_count-1].params[param_count] = type;
    print_functions();
}