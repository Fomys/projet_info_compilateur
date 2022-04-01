#include "program.h"

int pc;
struct instruction instructions[1000];



void push_instruction(enum instructionkind instruction, int op1, int op2, int op3) {
    //printf("%d %d %d %d\n", instruction, op1, op2, op3);
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
    FILE * fd = fopen("out.asm", "w");
    for(unsigned int i = 0; i < pc; i++) {
        switch(instructions[i].instruction) {
            case ADD:
                fprintf(fd, "ADD");
                break;
            case SOU:
                fprintf(fd, "SOU");
                break;
            case MUL:
                fprintf(fd, "MUL");
                break;
            case DIV:
                fprintf(fd, "DIV");
                break;
            case EQU:
                fprintf(fd, "EQU");
                break;
            case SUP:
                fprintf(fd, "SUP");
                break;
            case INF:
                fprintf(fd, "INF");
                break;
            case COP:
                fprintf(fd, "COP");
                break;
            case AFC:
                fprintf(fd, "AFC");
                break;
            case JMP:
                fprintf(fd, "JMP");
                break;
            case JMF:
                fprintf(fd, "JMF");
                break;
            case PRI:
                fprintf(fd, "PRI");
                break;
        }
        fprintf(fd, " %d %d %d\n", instructions[i].operandes[0], instructions[i].operandes[1], instructions[i].operandes[2]);
    }
}
void patch_instruction(int pc, int operand, int value) {
   //printf("old = %d %d %d %d\n", instructions[pc].instruction, instructions[pc].operandes[0], instructions[pc].operandes[1], instructions[pc].operandes[2]);
    instructions[pc].operandes[operand] = value;
    //printf("new = %d %d %d %d\n", instructions[pc].instruction, instructions[pc].operandes[0], instructions[pc].operandes[1], instructions[pc].operandes[2]);
}