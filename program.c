#include "program.h"

int pc;
struct instruction instructions[1000];



void push_instruction(enum instructionkind instruction, int op1, int op2, int op3) {
    instructions[pc].instruction = instruction;
    instructions[pc].operandes[0] = op1;
    instructions[pc].operandes[1] = op2;
    instructions[pc].operandes[2] = op3;
    pc++;
}

int get_pc() {
    return pc;
}

void program_print_asm() {
    for(unsigned int i = 0; i < pc; i++) {
        printf("%d - ", i);
        switch(instructions[i].instruction) {
            case ADD:
                printf("ADD %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case SOU:
                printf("SOU %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case MUL:
                printf("MUL %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case DIV:
                printf("DIV %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case EQU:
                printf("EQU %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case SUP:
                printf("SUP %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case INF:
                printf("INF %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
                break;
            case COP:
                printf("COP %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1]);
                break;
            case AFC:
                printf("AFC %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1]);
                break;
            case JMP:
                printf("JMP %d\n", instructions[i].operandes[0]);
                break;
            case JMF:
                printf("JMF %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1]);
                break;
            case PRI:
                printf("PRI %d\n", instructions[i].operandes[0]);
                break;
        }
    }
}

void patch_instruction(int pc, int operand, int value) {
instructions[pc].operandes[operand] = value;
}