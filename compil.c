#include "compil.h"
//#define DEBUG printf("%s\n", __FUNCTION__);
#define DEBUG
struct compil compil;

void compil_initialize() { DEBUG;
    compil.symbol_table = symbol_table_alloc();
    compil.function_table = function_table_alloc();
    compil.work_pointer = 10;
}

void compil_add_symbol(enum type type, char * name) { DEBUG;
    struct symbol * symbol = symbol_alloc();
    symbol->type = type;
    symbol_set_name(symbol, name);
    symbol_table_push_symbol(compil.symbol_table, symbol);
}

void compil_add_function(enum type type, char * name) { DEBUG;
    struct function * function = function_alloc();
    function_set_name(function, name);
    function->type = type;
    function_table_push_function(compil.function_table, function);
}

void compil_add_param(enum type type, char * name) { DEBUG;
    // Ajout du symbole dans la table
    compil_add_symbol(type, name);
    // Ajout de la fonction
    struct function * function = function_table_get_last_function(compil.function_table);
    function_add_param(function, type);
}

void compil_enter_scope() { DEBUG;
    symbol_table_enter_scope(compil.symbol_table);
}

void compil_exit_scope() { DEBUG;
    symbol_table_exit_scope(compil.symbol_table);
}

void compil_print_state() { DEBUG;
    symbol_table_print(compil.symbol_table);
    function_table_print(compil.function_table);
}

int compil_expr_push_nb(int nb) { DEBUG;
    compil.work_pointer ++;
    push_instruction(AFC, compil.work_pointer, nb, 0);
    return compil.work_pointer;
}

int compil_expr_push_var(char * name) { DEBUG;
    compil.work_pointer++;
    int src = symbol_table_get_address(compil.symbol_table, name);
    push_instruction(COP, compil.work_pointer, src, 0);
    return compil.work_pointer;
}

int compil_add(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(ADD, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_min(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(SOU, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_mul(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(MUL, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_div(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(DIV, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_eq(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(EQU, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_gt(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(SUP, compil.work_pointer, a, b);
    return compil.work_pointer;
}

int compil_lt(int a, int b) { DEBUG;
    compil.work_pointer--;
    push_instruction(INF, compil.work_pointer, a, b);
    return compil.work_pointer;
}


void compil_assign(char * name, int addr) { DEBUG;
    int dst = symbol_table_get_address(compil.symbol_table, name);
    push_instruction(COP, dst, addr, 0);
    compil.work_pointer--;
}


void compil_print_asm() { DEBUG;
    program_print_asm();
}

void compil_start_if(int addr) { DEBUG;
    push_instruction(JMF, addr, 0, 0);
}

void compil_patch_if(int p_if, int addr) { DEBUG;
    //modifie le JMF situé en p_if : l'argument 1 devient addr
    patch_instruction(p_if, 1, addr);
}

void compil_start_else() { DEBUG;
    push_instruction(JMP, 0, 0, 0);
}

void compil_patch_else(int p_else) { DEBUG;
    //modifie le JMP situé en p_else : l'argument 1 devient current pointeur
    patch_instruction(p_else, 0, get_pc());
}


void compil_start_while(int addr) { DEBUG;
    push_instruction(JMF, addr, 0, 0);
}

void compil_patch_while(int p_while, int p_jmp) { DEBUG;
    push_instruction(JMP, p_while, 0, 0);
    patch_instruction(p_jmp, 1, get_pc());
}

int compil_get_pc() { DEBUG;
    return get_pc();
}

void compil_print(int addr) { DEBUG;
    push_instruction(PRI, addr, 0, 0);
}

//void compil_assign(char * name, );