#ifndef INSTRUCTION_ARRAY_H
#define INSTRUCTION_ARRAY_H

#include "symbol_table.h"
#include <stdio.h>
#include <stdbool.h>
enum instruction_opcode {
    ADD, // op0 <= op1 + op2
    MUL, // op0 <= op1 * op2
    DIV, // op0 <= op1 / op2
    SUB, // op0 <= op1 - op2
    EQ, // op0 <= op1 == op2
    GT, // op0 <= op1 > op2
    LT, // op0 <= op1 < op2
    AFC, // op0 <= op1
    JMP, // dst = op0
    JMZ, // dst = op0, cond = op1
    CPY, // op0 <= op1
    NOP,
    PUSH, // src = op0
    POP, // dst = op0
    RET,
    CALL, // function = op0
    REGISTER_PROTECT, // Protège les registres
    SUBI,
    ADDI,
    END,
    LDR,
    STR
};

enum instruction_operand_kind {
    INSTRUCTION_OPERAND_CONSTANT,
    INSTRUCTION_OPERAND_SYMBOL,
    INSTRUCTION_OPERAND_INSTRUCTION,
    INSTRUCTION_OPERAND_REGISTER,
    INSTRUCTION_OPERAND_PARAMETERS,
    INSTRUCTION_OPERAND_STACK_SIZE
};

struct parameters {
    int parameter_count;
    struct symbol** parameters;
};

struct operand {
    enum instruction_operand_kind kind;
    union {
        struct symbol* symbol;
        int constant;
        int offset;
        struct instruction* instruction;
        int reg;
        struct parameters* parameters;
    };
};

struct instruction {
    struct instruction* before;
    struct instruction* after;
    bool deleted;
    enum instruction_opcode opcode;
    struct operand op0;
    struct operand op1;
    struct operand op2;
};

struct instruction_array {
    struct instruction* head;
    struct instruction* tail;
    struct instruction** deleted;
    unsigned int deleted_count;
};

struct instruction_array* instruction_array_alloc();
void instruction_array_free(struct instruction_array* array);
void instruction_array_print(struct instruction_array* array);
void instruction_array_print_raw(struct instruction_array* array, FILE* file);
void instruction_array_push(struct instruction_array* array, struct instruction* instruction);
void instruction_array_append(struct instruction_array* array, struct instruction_array* other);
void instruction_array_pop(struct instruction_array* array, struct instruction* instruction);

struct instruction* instruction_alloc();
void instruction_free(struct instruction* instruction);
void instruction_print(struct instruction*);
void instruction_insert_before(struct instruction_array* array, struct instruction* instruction, struct instruction* to_insert);
void instruction_insert_after(struct instruction_array* array, struct instruction* instruction, struct instruction* to_insert);

struct parameters* parameters_alloc();
void parameter_free(struct parameters* parameters);
void parameter_push(struct parameters* parameters, struct symbol* symbol);

void operand_print(struct operand*);
int instruction_array_search(struct instruction_array* array, struct instruction* instruction);

#endif //INSTRUCTION_ARRAY_H
