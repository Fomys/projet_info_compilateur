#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include "instruction_array.h"
#include "memory_manager.h"

struct instruction_array* instruction_array_alloc() {
    struct instruction_array* array = malloc(sizeof(struct instruction_array));
    if(array == NULL) {
        perror("Failed to malloc for instructions.\n");
        exit(errno);
    }
    array->head = NULL;
    array->tail = NULL;
    array->deleted = malloc(0);
    array->deleted_count = 0;
    return array;
}
void instruction_array_free(struct instruction_array* array) {
    struct instruction* next = array->head;
    while(next != NULL) {
        next = next->after;
        instruction_free(next->before);
    }
    free(array->deleted);
    free(array);
}
void instruction_array_print(struct instruction_array* array) {
    struct instruction* next = array->head;
    int i = 0;
    while(next != NULL) {
        printf("%d\t%p -> %p\t", i++, next, next->after);
        instruction_print(next);
        next = next->after;
    }
}

void instruction_array_print_raw(struct instruction_array* array, FILE* file) {
    struct instruction* next = array->head;
    while(next != NULL) {
        switch(next->opcode) {
            case ADD:
                fprintf(file, "ADD");
                break;
            case MUL:
                fprintf(file, "MUL");
                break;
            case DIV:
                fprintf(file, "DIV");
                break;
            case SUB:
                fprintf(file, "SUB");
                break;
            case EQ:
                fprintf(file, "EQ");
                break;
            case GT:
                fprintf(file, "GT");
                break;
            case LT:
                fprintf(file, "LT");
                break;
            case AFC:
                fprintf(file, "AFC");
                break;
            case JMP_I:
                fprintf(file, "JMPI");
                break;
            case JMZ_I:
                fprintf(file, "JMZI");
                break;
            case CPY:
                fprintf(file, "CPY");
                break;
            case NOP:
                fprintf(file, "NOP");
                break;
            case PUSH:
                fprintf(file, "PUSH");
                break;
            case POP:
                fprintf(file, "POP");
                break;
            case STR:
                fprintf(file, "STR");
                break;
            case LDR:
                fprintf(file, "LDR");
                break;
            case RET:
                fprintf(file, "RET");
                break;
            case CALL:
                fprintf(file, "CALL");
                break;
            case REGISTER_PROTECT:
                fprintf(file, "REGISTER_PROTECT");
                break;
            case END:
                fprintf(file, "END");
                break;
            case SUBI:
                fprintf(file, "SUBI");
                break;
            case ADDI:
                fprintf(file, "ADDI");
                break;
            case JMP:
                fprintf(file, "JMP");
                break;
            case LABEL:
                fprintf(file, "LABEL");
                break;
        }
        fprintf(file, " %d %d %d\n", next->op0.constant, next->op1.constant, next->op2.constant);
        next = next->after;
    }
}

void instruction_array_push(struct instruction_array* array, struct instruction* instruction) {
    if(array->head == NULL) {
        array->head = instruction;
        array->tail = instruction;
    } else {
        array->tail->after = instruction;
        instruction->before = array->tail;
        array->tail = instruction;
    }
}
void instruction_array_append(struct instruction_array* array, struct instruction_array* other) {
    if(other->head != NULL) {
        instruction_array_push(array, other->head);
        array->tail = other->tail;
    }
}

struct instruction* instruction_alloc() {
    struct instruction* instruction = malloc(sizeof(struct instruction));
    if(instruction == NULL) {
        perror("Failed to malloc for instruction.\n");
        exit(errno);
    }
    instruction->deleted = false;
    instruction->before = NULL;
    instruction->after = NULL;
    return instruction;
}
void instruction_free(struct instruction* instruction) {
    free(instruction);
}
void instruction_print(struct instruction* instruction) {
    switch (instruction->opcode) {
        case MUL:
            printf("MUL(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" * ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case DIV:
            printf("DIV(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" / ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case SUB:
            printf("SUB(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" - ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case EQ:
            printf("EQ(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" == ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case GT:
            printf("GT(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" > ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case LT:
            printf("LT(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" < ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case ADD:
            printf("ADD(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" + ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case AFC:
            printf("AFC(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(")");
            break;
        case JMP_I:
            printf("JMP(");
            operand_print(&instruction->op0);
            printf(")");
            break;
        case CPY:
            printf("CPY(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(")");
            break;
        case PUSH:
            printf("PUSH(");
            operand_print(&instruction->op0);
            printf(")");
            break;
        case POP:
            printf("POP(");
            operand_print(&instruction->op0);
            printf(")");
            break;
        case JMZ_I:
            printf("JMZ(");
            operand_print(&instruction->op0);
            printf(", to: ");
            operand_print(&instruction->op1);
            printf(")");
            break;
        case NOP:
            printf("NOP");
            break;
        case STR:
            printf("STR(");
            operand_print(&instruction->op0);
            printf(" => @");
            operand_print(&instruction->op1);
            printf(")");
            break;
        case LDR:
            printf("LDR(");
            operand_print(&instruction->op0);
            printf(" <= @");
            operand_print(&instruction->op1);
            printf(")");
            break;
        case RET:
            printf("RET");
            break;
        case CALL:
            printf("CALL(");
            operand_print(&instruction->op0);
            printf(")");
            break;
        case REGISTER_PROTECT:
            printf("REGISTER_PROTECT");
            break;
        case END:
            printf("END");
            break;
        case SUBI:
            printf("SUBI(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" - ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case ADDI:
            printf("ADDI(");
            operand_print(&instruction->op0);
            printf(" <= ");
            operand_print(&instruction->op1);
            printf(" + ");
            operand_print(&instruction->op2);
            printf(")");
            break;
        case LABEL:
            printf("LABEL(");
            operand_print(&instruction->op0);
            printf(")");
            break;
        case JMP:
            printf("JMP(@");
            operand_print(&instruction->op0);
            printf(")");
            break;
    }
    printf("\n");
}

void operand_print(struct operand* operand) {
    switch (operand->kind) {
        case INSTRUCTION_OPERAND_CONSTANT:
            printf("%d", operand->constant);
            break;
        case INSTRUCTION_OPERAND_SYMBOL:
            symbol_print(operand->symbol);
            break;
        case INSTRUCTION_OPERAND_INSTRUCTION:
            printf("%p", operand->instruction);
            break;
        case INSTRUCTION_OPERAND_REGISTER:
            if(operand->reg == PC_REGISTER) {
                printf("pc");
            } else if (operand->reg == SP_REGISTER) {
                printf("sp");
            } else {
                printf("r%d", operand->reg);
            }
            break;
        case INSTRUCTION_OPERAND_PARAMETERS:
            printf("parameters");
            break;
        case INSTRUCTION_OPERAND_STACK_SIZE:
            printf("stacksize");
            break;
    }
}

void instruction_insert_before(struct instruction_array* array, struct instruction* instruction, struct instruction* to_insert) {
    to_insert->before = instruction->before;
    to_insert->after = instruction;
    if(instruction == array->head) {
        array->head = to_insert;
    } else {
        instruction->before->after = to_insert;
    }
    instruction->before = to_insert;
}

void instruction_insert_after(struct instruction_array* array, struct instruction* instruction, struct instruction* to_insert) {
    to_insert->before = instruction;
    to_insert->after = instruction->after;
    if(instruction == array->tail) {
        array->tail = to_insert;
    } else {
        instruction->after->before = to_insert;
    }
    instruction->after = to_insert;
}

void instruction_array_pop(struct instruction_array* array, struct instruction* instruction) {
    if(instruction == array->head) {
        array->head = instruction->after;
        array->head->before = NULL;
    } else {
        instruction->before->after = instruction->after;
    }
    if(instruction == array->tail) {
        array->tail = instruction->before;
        array->tail->after = NULL;
    } else {
        instruction->after->before = instruction->before;
    }
    array->deleted_count++;
    array->deleted = reallocarray(array->deleted, array->deleted_count, sizeof(struct instruction*));
    if(array->deleted == NULL) {
        printf("Failed to reallocarray for instructionarray.\n");
        exit(errno);
    }
    instruction->deleted = true;
    array->deleted[array->deleted_count-1] = instruction;
}


struct parameters* parameters_alloc() {
    struct parameters* parameters = malloc(sizeof(struct parameters));
    if(parameters == NULL) {
        perror("Failed to malloc for parameters.\n");
        exit(errno);
    }
    parameters->parameters = malloc(0);
    parameters->parameter_count = 0;
    return parameters;
}

void parameter_free(struct parameters* parameters) {
    free(parameters->parameters);
    free(parameters);
}

void parameter_push(struct parameters* parameters, struct symbol* symbol) {
    parameters->parameter_count++;
    parameters->parameters = reallocarray(parameters->parameters, parameters->parameter_count, sizeof(struct symbol*));
    if(parameters->parameters == NULL) {
        perror("Failed to reallocarray for parameters.\n");
        exit(errno);
    }
    parameters->parameters[parameters->parameter_count - 1] = symbol;
}

int instruction_array_search(struct instruction_array* array, struct instruction* instruction) {
    struct instruction* next = array->head;
    int i = -1;
    while(next != NULL) {
        i += 1;
        if(next == instruction) {
            return i;
        }
        next = next->after;
    }
    return i;
}