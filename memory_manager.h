#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdbool.h>
#include "instruction_array.h"
#include "ast.h"

#define GENERAL_REGISTER_COUNT 14
#define SP_REGISTER (GENERAL_REGISTER_COUNT)
#define PC_REGISTER (SP_REGISTER + 1)

struct memory_manager {
    struct symbol* general_registers[GENERAL_REGISTER_COUNT];
    bool available_registers[GENERAL_REGISTER_COUNT];
    struct symbol** stack;
    int stack_size;
};

struct memory_manager* memory_manager_alloc();
void memory_manager_free(struct memory_manager*);
int memory_manager_find_free_register(struct memory_manager* manager, struct instruction* instruction);
int memory_manager_get_register(struct memory_manager* manager, struct instruction_array* array, struct instruction* instruction, struct symbol* symbol);
void memory_manager_associate_register(struct memory_manager* manager, struct instruction_array* array, struct instruction* instruction);
int memory_manager_find_allowed_free_register(struct memory_manager* manager, struct instruction* instruction);
void memory_manager_protect_registers(struct memory_manager* manager, struct instruction_array* array, struct instruction* instruction);
void manager_push_symbol_on_stack(struct memory_manager* manager, struct symbol* symbol);

#endif //MEMORY_MANAGER_H
