#include <string.h>
#include "assembler.h"
#include "instruction_array.h"

#define SKIP_RESOLVE_STACK 0
#define SKIP_RESOLVE_JMP 0
#define SKIP_ASSOCIATE_REGISTERS 0
#define SKIP_FIX_INSTRUCTIONS 0

void simplify_instruction(struct instruction_array* array, struct instruction* instruction) {
    switch (instruction->opcode) {
        case ADD:
        case MUL:
        case DIV:
        case SUB:
        case EQ:
        case GT:
        case LT:
        case RET:
        case JMP:
        case JMZ:
        case AFC:
        case NOP:
        case PUSH:
        case POP:
        case LDR:
        case STR:
        case CALL:
        case REGISTER_PROTECT:
        case END:
        case SUBI:
        case ADDI:
            break;
    }
}
void simplify_asm(struct instruction_array* array) {
    struct instruction* next = array->head;
    while(next != NULL) {
        simplify_instruction(array, next);
        next = next->after;
    }
}

void fix_instructions(struct instruction_array* array) {
#if SKIP_FIX_INSTRUCTIONS
    return;
#endif
    struct instruction* next = array->head;
    while(next != NULL) {
        if(next->op0.kind == INSTRUCTION_OPERAND_INSTRUCTION) {
            while(next->op0.instruction != NULL && next->op0.instruction->deleted == true) {
                next->op0.instruction = next->op0.instruction->after;
            }
        }
        if(next->op1.kind == INSTRUCTION_OPERAND_INSTRUCTION) {
            while(next->op1.instruction != NULL && next->op1.instruction->deleted == true) {
                next->op1.instruction = next->op1.instruction->after;
            }
        }if(next->op2.kind == INSTRUCTION_OPERAND_INSTRUCTION) {
            while(next->op2.instruction != NULL && next->op2.instruction->deleted == true) {
                next->op2.instruction = next->op2.instruction->after;
            }
        }
        next = next->after;
    }
}

void resolve_stack(struct instruction_array* array, struct memory_manager* manager) {
#if SKIP_RESOLVE_STACK
        return;
#endif
    struct instruction* next = array->head;
    while(next != NULL) {
        if(next->op0.kind == INSTRUCTION_OPERAND_STACK_SIZE) {
            next->op0.kind = INSTRUCTION_OPERAND_CONSTANT;
            next->op0.constant = next->op0.offset + manager->stack_size;
        }
        if (next->op1.kind == INSTRUCTION_OPERAND_STACK_SIZE) {
            next->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
            next->op1.constant = next->op1.offset + manager->stack_size;
        }
        if (next->op2.kind == INSTRUCTION_OPERAND_STACK_SIZE) {
            next->op2.kind = INSTRUCTION_OPERAND_CONSTANT;
            next->op2.constant = next->op2.offset + manager->stack_size;
        }
        next = next->after;
    }
}

void resolve_jmp(struct instruction_array* array) {
#if SKIP_RESOLVE_JMP
        return;
#endif
    struct instruction* next = array->head;
    while(next != NULL) {
        if(next->opcode == JMP && next->op0.kind == INSTRUCTION_OPERAND_INSTRUCTION) {
            next->op0.kind = INSTRUCTION_OPERAND_CONSTANT;
            next->op0.constant = instruction_array_search(array, next->op0.instruction->after);
        } else if(next->opcode == JMZ && next->op1.kind == INSTRUCTION_OPERAND_INSTRUCTION) {
            next->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
            next->op1.constant = instruction_array_search(array, next->op1.instruction->after);
        }
        next = next->after;
    }
}

struct instruction_array* assemble(struct AST_node* root) {
    AST_optimize(root);
    struct instruction_array* array = instruction_array_alloc();
    assemble_node(root, root->node_body.symbol_table, array);
    struct instruction* end = instruction_alloc();
    end->opcode = END;
    instruction_array_push(array, end);
    struct memory_manager* manager = memory_manager_alloc();
    associate_registers(manager, array);
    resolve_functions(array, manager);
    fix_instructions(array);
    simplify_asm(array);
    resolve_stack(array, manager);
    resolve_jmp(array);
    return array;
}



void resolve_functions(struct instruction_array* array, struct memory_manager* manager) {
    struct instruction* next = array->head;
    while(next != NULL) {
        if (next->opcode == CALL) {
            struct instruction* push_pc = instruction_alloc();
            push_pc->opcode = STR;
            push_pc->op0.kind = INSTRUCTION_OPERAND_REGISTER;
            push_pc->op0.reg = PC_REGISTER;
            push_pc->op1.kind = INSTRUCTION_OPERAND_REGISTER;
            push_pc->op1.constant = SP_REGISTER;
            instruction_insert_before(array, next, push_pc);
            next->opcode = JMP;
            if(next->op0.symbol->symbol_function.assembled == NULL) {
                AST_optimize(next->op0.symbol->symbol_function.body);
                struct instruction_array *function_array = instruction_array_alloc();
                struct instruction* nop = instruction_alloc();
                nop->opcode = NOP;
                instruction_array_push(function_array, nop);
                struct memory_manager *manager = memory_manager_alloc();
                struct symbol* anonymous = symbol_alloc();
                manager_push_symbol_on_stack(manager, anonymous);

                assemble_node(next->op0.symbol->symbol_function.body,
                              next->op0.symbol->symbol_function.body->node_body.symbol_table, function_array);

                // Récupération des arguments
                for(unsigned int i = 0; i < next->op0.symbol->symbol_function.arguments->arguments_count; i++) {
                    manager_push_symbol_on_stack(manager, next->op0.symbol->symbol_function.arguments->arguments[i]);
                }
                associate_registers(manager, function_array);


                next->op0.symbol->symbol_function.assembled = function_array;
                next->op0.kind = INSTRUCTION_OPERAND_INSTRUCTION;
                next->op0.instruction = function_array->head;
                resolve_functions(function_array, manager);
                resolve_stack(function_array, manager);
                instruction_array_append(array, function_array);
            } else {
                next->op0.kind = INSTRUCTION_OPERAND_INSTRUCTION;
                next->op0.instruction = next->op0.symbol->symbol_function.assembled->head;
            }
        }
        next = next->after;
    }
}

void associate_registers(struct memory_manager* manager, struct instruction_array* array) {
#if SKIP_ASSOCIATE_REGISTERS
        return;
#endif
    struct instruction* next = array->head;
    while(next != NULL) {
        struct instruction *after = next->after;
        memory_manager_associate_register(manager, array, next);
        next = after;
    }
}

struct symbol* assemble_body(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    for(unsigned int i = 0; i < node->node_body.children_count; i++) {
        assemble_node(node->node_body.children[i], symbol_table, instructions);
    }
    return NULL;
}

struct symbol* assemble_operation(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    struct symbol* left = assemble_node(node->node_operation.left, symbol_table, instructions);
    struct symbol* right = assemble_node(node->node_operation.right, symbol_table, instructions);
    struct symbol* anonymous = symbol_table_create_anonymous(symbol_table);
    struct instruction* instruction = instruction_alloc();
    switch (node->node_operation.op) {
        case OP_ADD:
            instruction->opcode = ADD;
            break;
        case OP_SUB:
            instruction->opcode = SUB;
            break;
        case OP_MUL:
            instruction->opcode = MUL;
            break;
        case OP_DIV:
            instruction->opcode = DIV;
            break;
        case OP_EQ:
            instruction->opcode = EQ;
            break;
        case OP_GT:
            instruction->opcode = GT;
            break;
        case OP_LT:
            instruction->opcode = LT;
            break;
    }
    instruction->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op0.symbol = anonymous;
    instruction->op0.symbol->usage_count++;
    instruction->op1.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op1.symbol = left;
    instruction->op1.symbol->usage_count++;
    instruction->op2.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op2.symbol = right;
    instruction->op2.symbol->usage_count++;
    instruction_array_push(instructions, instruction);
    return anonymous;
}

struct symbol* assemble_integer(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    struct symbol* anonymous = symbol_table_create_anonymous(symbol_table);
    struct instruction* instruction = instruction_alloc();
    instruction->opcode = AFC;
    instruction->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op0.symbol = anonymous;
    instruction->op0.symbol->usage_count++;
    instruction->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    instruction->op1.constant = node->node_integer.value;
    instruction_array_push(instructions, instruction);
    return anonymous;
}

struct symbol* assemble_assignement(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    struct symbol* anonymous = symbol_table_create_anonymous(symbol_table);
    anonymous->usage_count++;
    struct instruction* instruction = instruction_alloc();
    instruction->opcode = CPY;
    instruction->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op0.symbol = node->node_assignement.symbol;
    instruction->op0.symbol->usage_count++;
    instruction->op1.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op1.symbol = assemble_node(node->node_assignement.value, symbol_table, instructions);
    instruction->op1.symbol->usage_count++;
    instruction_array_push(instructions, instruction);
    return anonymous;
}

struct symbol* assemble_function_call(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    // Calcul des paramètres
    struct parameters* parameters = parameters_alloc();
    for(unsigned int i = 0; i < node->node_call.parameters->parameters_count; i++) {
        struct symbol* param = assemble_node(node->node_call.parameters->parameters[i], symbol_table, instructions);
        parameter_push(parameters, param);
    }
    // Protection des registres
    struct instruction* protect = instruction_alloc();
    protect->opcode = REGISTER_PROTECT;
    instruction_array_push(instructions, protect);

    // Incrémentation de la stack
    struct instruction* increase = instruction_alloc();
    increase->opcode = ADDI;
    increase->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    increase->op0.offset = SP_REGISTER;
    increase->op1.kind = INSTRUCTION_OPERAND_INSTRUCTION;
    increase->op1.offset = 0;
    instruction_array_push(instructions,increase);

    for(int i = 0; i < parameters->parameter_count; i++) {
        struct instruction *push = instruction_alloc();
        push->opcode = ADDI;
        push->op0.kind = INSTRUCTION_OPERAND_REGISTER;
        push->op0.reg = SP_REGISTER;
        push->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
        push->op1.constant = i+1;
        instruction_array_push(instructions, push);
        push->opcode = STR;
        push->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
        push->op0.symbol = parameters->parameters[i];
        push->op1.kind = INSTRUCTION_OPERAND_REGISTER;
        push->op1.constant = SP_REGISTER; //i + 1;
        instruction_array_push(instructions, push);
        push->opcode = SUBI;
        push->op0.kind = INSTRUCTION_OPERAND_REGISTER;
        push->op0.reg = SP_REGISTER;
        push->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
        push->op1.constant = i+1;
        instruction_array_push(instructions, push);
    }

    // Appel de la fonction
    struct symbol* anonymous = symbol_table_create_anonymous(symbol_table);
    struct instruction* call = instruction_alloc();
    call->opcode = CALL;
    call->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    call->op0.symbol = node->node_symbol.symbol;
    instruction_array_push(instructions, call);
    // Récupération de la valeur retour
    struct instruction* ret = instruction_alloc();
    ret->opcode = ADDI;
    ret->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op0.reg = SP_REGISTER;
    ret->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    ret->op1.constant = 1;
    instruction_array_push(instructions, ret);
    ret->opcode = LDR;
    ret->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    ret->op0.symbol = anonymous;
    ret->op0.symbol->usage_count++;
    ret->op1.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op1.reg = SP_REGISTER;
    instruction_array_push(instructions, ret);
    ret->opcode = SUBI;
    ret->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op0.reg = SP_REGISTER;
    ret->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    ret->op1.constant = 1;
    instruction_array_push(instructions, ret);
    // Décrémentation de la stack
    struct instruction* decrease = instruction_alloc();
    decrease->opcode = SUBI;
    decrease->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    decrease->op0.reg = SP_REGISTER;
    decrease->op1.kind = INSTRUCTION_OPERAND_STACK_SIZE;
    decrease->op1.offset = 0;
    instruction_array_push(instructions, decrease);
    return anonymous;
}

struct symbol* assemble_if(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* instructions) {
    struct symbol* condition = assemble_node(node->node_if.condition, symbol_table, instructions);

    struct instruction* instruction_jmp_false = instruction_alloc();
    struct instruction* instruction_jmp_true = instruction_alloc();

    instruction_jmp_false->opcode = JMZ;
    instruction_jmp_false->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction_jmp_false->op0.symbol = condition;
    instruction_jmp_false->op0.symbol->usage_count++;
    instruction_jmp_false->op1.kind = INSTRUCTION_OPERAND_INSTRUCTION;
    instruction_array_push(instructions, instruction_jmp_false);

    assemble_node(node->node_if.if_true, symbol_table, instructions);

    instruction_jmp_true->opcode = JMP;
    instruction_jmp_true->op0.kind = INSTRUCTION_OPERAND_INSTRUCTION;
    instruction_array_push(instructions, instruction_jmp_true);

    assemble_node(node->node_if.if_false, symbol_table, instructions);
    instruction_jmp_false->op1.instruction = instruction_jmp_true;
    instruction_jmp_true->op0.instruction = instructions->tail;
    return NULL;
}

struct symbol* assemble_declaration(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* array) {
    struct instruction* instruction = instruction_alloc();
    instruction->opcode = AFC;
    instruction->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    instruction->op0.symbol = node->node_declaration.symbol;
    instruction->op0.symbol->usage_count++;
    instruction->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    instruction->op1.constant = 0;
    instruction_array_push(array, instruction);
    return NULL;
}

struct symbol* assemble_return(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* array) {
    struct instruction* ret = instruction_alloc();
    ret->opcode = ADDI;
    ret->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op0.reg = SP_REGISTER;
    ret->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    ret->op1.constant = 1;
    instruction_array_push(array, ret);
    ret->opcode = STR;
    ret->op0.kind = INSTRUCTION_OPERAND_SYMBOL;
    ret->op0.symbol = assemble_node(node->node_return.expression, symbol_table, array);
    ret->op0.symbol->usage_count++;
    ret->op1.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op1.constant = SP_REGISTER;
    instruction_array_push(array, ret);
    ret = instruction_alloc();
    ret->opcode = SUBI;
    ret->op0.kind = INSTRUCTION_OPERAND_REGISTER;
    ret->op0.reg = SP_REGISTER;
    ret->op1.kind = INSTRUCTION_OPERAND_CONSTANT;
    ret->op1.constant = 1;
    instruction_array_push(array, ret);
    ret = instruction_alloc();
    ret->opcode = RET;
    instruction_array_push(array, ret);
    return NULL;
}

struct symbol* assemble_node(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* array) {
    switch (node->kind) {
        case NODE_BODY:
            return assemble_body(node, node->node_body.symbol_table, array);
        case NODE_OPERATION:
            return assemble_operation(node, symbol_table, array);
        case NODE_INTEGER:
            return assemble_integer(node, symbol_table, array);
        case NODE_SYMBOL:
            return node->node_symbol.symbol;
        case NODE_ASSIGNEMENT:
            return assemble_assignement(node, symbol_table, array);
        case NODE_CALL:
            return assemble_function_call(node, symbol_table, array);
        case NODE_IF:
            return assemble_if(node, symbol_table, array);
        case NODE_DECLARATION:
            return assemble_declaration(node, symbol_table, array);
        case NODE_NOP:
            break;
        case NODE_RETURN:
            return assemble_return(node, symbol_table, array);
    }
    return NULL;
}