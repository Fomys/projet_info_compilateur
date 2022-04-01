#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>

enum instructionkind {
    ADD = 0, 
    SOU = 1,
    MUL = 2,
    DIV = 3,
    EQU = 4,
    SUP = 5,
    INF = 6,
    COP = 7,
    AFC = 8,
    JMP = 9,
    JMF = 10,
    PRI = 11,
};

struct instruction {
    enum instructionkind instruction; 
    int operandes[3];
};

void push_instruction(enum instructionkind instruction, int op1, int op2, int op3);
void patch_instruction(int pc, int operand, int value);
int get_pc();

void program_print_asm();


#endif //PROGRAM_H