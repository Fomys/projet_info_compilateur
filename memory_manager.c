#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include "memory_manager.h"

struct memory_manager *memory_manager_alloc() {
    struct memory_manager *manager = malloc(sizeof(struct memory_manager));
    if (manager == NULL) {
        perror("Failled to malloc for memory_manager.\n");
        exit(errno);
    }
    for (unsigned int i = 0; i < GENERAL_REGISTER_COUNT; i++) {
        manager->general_registers[i] = NULL;
        manager->available_registers[i] = true;
    }
    manager->stack = malloc(0);
    manager->stack_size = 0;
    return manager;
}

void memory_manager_free(struct memory_manager *manager) {
    free(manager->stack);
    free(manager);
}


int memory_manager_find_free_stack(struct memory_manager *manager) {
    for (int i = 0; i < manager->stack_size; i++) {
        if (manager->stack[i] == NULL) {
            return i;
        }
    }
    manager->stack_size++;
    manager->stack = reallocarray(manager->stack, manager->stack_size, sizeof(struct symbol *));
    if (manager->stack == NULL) {
        perror("Failed to reallocarray for memory_manager.\n");
        exit(errno);
    }
    return manager->stack_size - 1;
}

int memory_manager_search_register(struct memory_manager *manager, struct symbol *symbol) {
    for (int i = 0; i < GENERAL_REGISTER_COUNT; i++) {
        if (manager->general_registers[i] == symbol) {
            return i;
        }
    }
    return -1;
}

int memory_manager_release_register(struct memory_manager *manager, struct instruction_array *array,
                                    struct instruction *instruction, int reg) {
    int free_stack = memory_manager_find_free_stack(manager);
    struct instruction *stack_set = instruction_alloc();
    stack_set->opcode = ADDI;
    stack_set->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op0.reg = SP_REGISTER;
    stack_set->op1.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op1.reg = SP_REGISTER;
    stack_set->op2.kind = INSTRUCTION_OPERAND_CONSTANT;
    stack_set->op2.constant = free_stack;
    instruction_insert_before(array, instruction, stack_set);
    stack_set = instruction_alloc();
    stack_set->opcode = STR;
    stack_set->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op0.reg = reg;
    stack_set->op1.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op1.constant = SP_REGISTER;
    instruction_insert_before(array, instruction, stack_set);
    stack_set = instruction_alloc();
    stack_set->opcode = SUBI;
    stack_set->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op0.reg = SP_REGISTER;
    stack_set->op1.kind = INSTRUCTION_OPERAND_REGISTER;
    stack_set->op1.reg = SP_REGISTER;
    stack_set->op2.kind = INSTRUCTION_OPERAND_CONSTANT;
    stack_set->op2.constant = free_stack;
    instruction_insert_before(array, instruction, stack_set);
    manager->stack[free_stack] = manager->general_registers[reg];
    manager->available_registers[reg] = true;
    return reg;
}

int memory_manager_get_register(struct memory_manager *manager, struct instruction_array *array,
                                struct instruction *instruction, struct symbol *symbol) {
    int reg;
    reg = memory_manager_search_register(manager, symbol);
    if (reg != -1) {
        return reg;
    }
    for (int i = 0; i < manager->stack_size; i++) {
        if (manager->stack[i] == symbol) {
            int free_register = memory_manager_find_free_register(manager, instruction);
            if (free_register == -1) {
                free_register = memory_manager_release_register(manager, array, instruction,
                                                                memory_manager_find_allowed_free_register(manager,
                                                                                                          instruction));
            }
            struct instruction *stack_get = instruction_alloc();
            stack_get->opcode = ADDI;
            stack_get->op0.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op0.reg = SP_REGISTER;
            stack_get->op1.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op1.reg = SP_REGISTER;
            stack_get->op2.kind = INSTRUCTION_OPERAND_CONSTANT;
            stack_get->op2.constant = i;
            instruction_insert_before(array, instruction, stack_get);
            stack_get = instruction_alloc();
            stack_get->opcode = LDR;
            stack_get->op0.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op0.reg = free_register;
            stack_get->op1.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op1.constant = SP_REGISTER;
            instruction_insert_before(array, instruction, stack_get);
            stack_get = instruction_alloc();
            stack_get->opcode = SUBI;
            stack_get->op0.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op0.reg = SP_REGISTER;
            stack_get->op1.kind = INSTRUCTION_OPERAND_REGISTER;
            stack_get->op1.reg = SP_REGISTER;
            stack_get->op2.kind = INSTRUCTION_OPERAND_CONSTANT;
            stack_get->op2.constant = i;
            instruction_insert_before(array, instruction, stack_get);
            manager->general_registers[free_register] = manager->stack[i];
            manager->available_registers[free_register] = false;
            manager->stack[i] = NULL;
            return free_register;
        }
    }
    int free_register = memory_manager_find_free_register(manager, instruction);
    if (free_register == -1) {
        free_register = memory_manager_release_register(manager, array, instruction,
                                                        memory_manager_find_allowed_free_register(manager,
                                                                                                  instruction));
    }
    manager->general_registers[free_register] = symbol;
    manager->available_registers[free_register] = false;
    return free_register;
}


bool memory_manager_register_is_freeable(struct memory_manager *manager, struct instruction* instruction, int reg) {
    if (!((instruction->op0.kind == INSTRUCTION_OPERAND_SYMBOL &&
              instruction->op0.symbol == manager->general_registers[reg]) ||
             (instruction->op1.kind == INSTRUCTION_OPERAND_SYMBOL &&
              instruction->op1.symbol == manager->general_registers[reg]) ||
             (instruction->op2.kind == INSTRUCTION_OPERAND_SYMBOL &&
              instruction->op2.symbol == manager->general_registers[reg]) ||
             (instruction->op0.kind == INSTRUCTION_OPERAND_REGISTER && instruction->op0.reg == reg) ||
             (instruction->op1.kind == INSTRUCTION_OPERAND_REGISTER && instruction->op1.reg == reg) ||
             (instruction->op2.kind == INSTRUCTION_OPERAND_REGISTER && instruction->op2.reg == reg))) {
        return true;
    } else {
        return false;
    }
}

int memory_manager_find_allowed_free_register(struct memory_manager *manager, struct instruction *instruction) {
    for(int i = 0; i < GENERAL_REGISTER_COUNT; i++) {
        if (memory_manager_register_is_freeable(manager, instruction, i))
            return i;
    }
    return -1;
}
int memory_manager_find_free_register(struct memory_manager *manager, struct instruction * instruction) {
    for (int i = 0; i < GENERAL_REGISTER_COUNT; i++) {
        if (manager->available_registers[i] == true || (manager->general_registers[i]->usage_count == 0 && memory_manager_register_is_freeable(manager, instruction, i))) {
            return i;
        }
    }
    return -1;
}


void memory_manager_associate_register(struct memory_manager *manager, struct instruction_array *array,
                                       struct instruction *instruction) {
    switch (instruction->opcode) {
        case ADD:
        case GT:
        case LT:
        case MUL:
        case DIV:
        case SUB:
        case EQ:
            if (instruction->op2.kind == INSTRUCTION_OPERAND_SYMBOL) {
                instruction->op2.symbol->usage_count--;
                instruction->op2.reg = memory_manager_get_register(manager, array, instruction,
                                                                   instruction->op2.symbol);
                instruction->op2.kind = INSTRUCTION_OPERAND_REGISTER;
            }
        case CPY:
            if (instruction->op1.kind == INSTRUCTION_OPERAND_SYMBOL) {
                instruction->op1.symbol->usage_count--;
                instruction->op1.reg = memory_manager_get_register(manager, array, instruction,
                                                                   instruction->op1.symbol);
                instruction->op1.kind = INSTRUCTION_OPERAND_REGISTER;
            }
        case PUSH:
        case POP:
        case AFC:
        case JMZ_I:
        case LDR:
        case STR:
            if (instruction->op0.kind == INSTRUCTION_OPERAND_SYMBOL) {
                instruction->op0.symbol->usage_count--;
                instruction->op0.reg = memory_manager_get_register(manager, array, instruction,
                                                                   instruction->op0.symbol);
                instruction->op0.kind = INSTRUCTION_OPERAND_REGISTER;
            }
        case JMP_I:
        case CALL:
        case NOP:
        case RET:
        case END:
        case SUBI:
        case ADDI:
            break;
        case REGISTER_PROTECT:
            memory_manager_protect_registers(manager, array, instruction);
            break;
    }
}

void memory_manager_protect_registers(struct memory_manager *manager, struct instruction_array *array,
                                      struct instruction *instruction) {
    for (int i = 0; i < GENERAL_REGISTER_COUNT; i++) {
        if (manager->general_registers[i] != NULL && manager->available_registers[i] == false) { //TODO vérifier utilisation
            memory_manager_release_register(manager, array, instruction, i);
        }
    }
    instruction_array_pop(array, instruction);
}

void manager_push_symbol_on_stack(struct memory_manager *manager, struct symbol *symbol) {
    int index = memory_manager_find_free_stack(manager);
    manager->stack[index] = symbol;
}