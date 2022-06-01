#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

#define PRINT_SPACES(offset) printf("%*s", (offset)*2, "")

void _AST_node_print(struct AST_node *node, int offset);

void AST_node_print(struct AST_node *node) {
    _AST_node_print(node, 0);
}

void _AST_node_print(struct AST_node *node, int offset) {
    PRINT_SPACES(offset);
    switch (node->kind) {
        case NODE_INTEGER:
            printf("INT: %d\n", node->node_integer.value);
            break;
        case NODE_OPERATION:
            switch (node->node_operation.op) {
                case OP_ADD:
                    printf("ADD");
                    break;
                case OP_SUB:
                    printf("SUB");
                    break;
                case OP_MUL:
                    printf("MUL");
                    break;
                case OP_DIV:
                    printf("DIV");
                    break;
                case OP_EQ:
                    printf("EQ");
                    break;
                case OP_GT:
                    printf("DIV");
                    break;
                case OP_LT:
                    printf("LT");
                    break;
            }
            printf(":\n");
            PRINT_SPACES(offset + 1);
            printf("left:\n");
            _AST_node_print(node->node_operation.left, offset + 2);
            PRINT_SPACES(offset + 1);
            printf("right:\n");
            _AST_node_print(node->node_operation.right, offset + 2);
            break;
        case NODE_ASSIGNEMENT:
            printf("ASSIGN ");
            symbol_print(node->node_assignement.symbol);
            printf(":\n");
            _AST_node_print(node->node_assignement.value, offset + 1);
            break;
        case NODE_BODY:
            printf("BODY:\n");
            for (unsigned int i = 0; i < node->node_body.children_count; i++) {
                _AST_node_print(node->node_body.children[i], offset + 1);
            }
            break;
        case NODE_SYMBOL:
            symbol_print(node->node_symbol.symbol);
            printf("\n");
            break;
        case NODE_CALL:
            printf("CALL %s:\n", node->node_call.symbol->name);
            for (unsigned int i = 0; i < node->node_call.parameters->parameters_count; i++) {
                PRINT_SPACES(offset + 1);
                printf("parameters: \n");
                _AST_node_print(node->node_call.parameters->parameters[i], offset + 2);
            }
            break;
        case NODE_IF:
            printf("IF:");
            PRINT_SPACES(offset + 1);
            printf("condition:");
            _AST_node_print(node->node_if.condition, offset + 2);
            PRINT_SPACES(offset + 1);
            printf("if true:\n");
            _AST_node_print(node->node_if.if_true, offset + 2);
            PRINT_SPACES(offset + 1);
            printf("if false:\n");
            _AST_node_print(node->node_if.if_false, offset + 2);
            break;
        case NODE_DECLARATION:
            printf("Declaration:");
            symbol_print(node->node_declaration.symbol);
            printf("\n");
            break;
        case NODE_NOP:
            printf("NOP\n");
            break;
        case NODE_RETURN:
            printf("RETURN:\n");
            _AST_node_print(node->node_return.expression, offset + 1);
            break;
    }
}


struct AST_node *AST_node_assignement_from(struct symbol *symbol, struct AST_node *value) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_ASSIGNEMENT;
    node->node_assignement.symbol = symbol;
    node->node_assignement.value = value;
    return node;
}


void AST_node_integer_initialize(struct AST_node *node, int value) {
    node->kind = NODE_INTEGER;
    node->node_integer.value = value;
}

void AST_node_body_add_child(struct AST_node *body, struct AST_node *child) {
    // TODO: Vérification pour body->kind == NODE_BODY
    body->node_body.children_count++;
    body->node_body.children = reallocarray(body->node_body.children, body->node_body.children_count,
                                            sizeof(struct AST_node *));
    if (body->node_body.children == NULL) {
        perror("Failed to reallocarray for new child.\n");
        exit(errno);
    }
    body->node_body.children[body->node_body.children_count - 1] = child;
}

struct AST_node *AST_node_operation_from(enum AST_op op, struct AST_node *left, struct AST_node *right) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_OPERATION;
    node->node_operation.op = op;
    node->node_operation.left = left;
    node->node_operation.right = right;
    return node;
}

struct AST_node* AST_node_return_from(struct AST_node* expression) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_RETURN;
    node->node_return.expression = expression;
    return node;
}

struct AST_node *AST_node_symbol_from(struct symbol *symbol) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_SYMBOL;
    node->node_symbol.symbol = symbol;
    return node;
}


void call_parameters_add(struct call_parameters *parameters, struct AST_node *node) {
    parameters->parameters_count++;
    parameters->parameters = reallocarray(parameters->parameters, parameters->parameters_count,
                                          sizeof(struct call_parameters *));
    parameters->parameters[parameters->parameters_count - 1] = node;
}

struct AST_node *AST_node_call_from(struct symbol *symbol, struct call_parameters *parameters) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_CALL;
    node->node_call.symbol = symbol;
    node->node_call.parameters = parameters;
    return node;
}

void AST_set_symbol_table(struct AST_node *node, struct symbol_table *symbol_table) {
    node->node_body.symbol_table = symbol_table;
}

struct AST_node *
AST_node_if_else_from(struct AST_node *condition, struct AST_node *body_if_true, struct AST_node *body_if_false) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_IF;
    node->node_if.condition = condition;
    node->node_if.if_true = body_if_true;
    node->node_if.if_false = body_if_false;
    return node;
}

struct AST_node *AST_node_declaration_from(struct symbol *symbol) {
    struct AST_node *node = AST_node_alloc();
    node->kind = NODE_DECLARATION;
    node->node_declaration.symbol = symbol;
    return node;
}