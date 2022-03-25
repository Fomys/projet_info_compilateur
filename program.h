#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>

enum instructionkind {
    ADD, SOU, MUL, DIV, EQU, SUP, INF, COP, AFC, JMP, JMF, PRI
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