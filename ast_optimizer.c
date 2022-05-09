#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void AST_optimize_operation(struct AST_node *node) {
    AST_optimize(node->node_operation.left);
    AST_optimize(node->node_operation.right);

    if (node->node_operation.left->kind == NODE_INTEGER &&
        node->node_operation.right->kind == NODE_INTEGER) { // cst op cst => cst
        int new_value = 0;
        switch (node->node_operation.op) {
            case OP_ADD:
                new_value =
                        node->node_operation.left->node_integer.value + node->node_operation.right->node_integer.value;
                break;
            case OP_SUB:
                new_value =
                        node->node_operation.left->node_integer.value - node->node_operation.right->node_integer.value;
                break;
            case OP_MUL:
                new_value =
                        node->node_operation.left->node_integer.value * node->node_operation.right->node_integer.value;
                break;
            case OP_DIV:
                if (node->node_operation.right->node_integer.value == 0) {
                    fprintf(stderr, "Compiler error: divide by 0.\n");
                    exit(1);
                }
                new_value =
                        node->node_operation.left->node_integer.value / node->node_operation.right->node_integer.value;
                break;
            case OP_EQ:
                new_value =
                        node->node_operation.left->node_integer.value == node->node_operation.right->node_integer.value;
                break;
            case OP_GT:
                new_value =
                        node->node_operation.left->node_integer.value > node->node_operation.right->node_integer.value;
                break;
            case OP_LT:
                new_value =
                        node->node_operation.left->node_integer.value < node->node_operation.right->node_integer.value;
                break;
        }
        node->kind = NODE_INTEGER;
        AST_node_free(node->node_operation.left);
        AST_node_free(node->node_operation.right);
        node->node_integer.value = new_value;
    } else if (node->node_operation.left->kind == NODE_INTEGER &&
               node->node_operation.left->node_integer.value == 0) { // 0 op cst => cst
        switch (node->node_operation.op) {
            case OP_ADD:
                AST_node_free(node->node_operation.left);
                *node = *node->node_operation.right;
                break;
            case OP_MUL:
            case OP_DIV:
                AST_node_free(node->node_operation.left);
                AST_node_free(node->node_operation.right);
                node->kind = NODE_INTEGER;
                node->node_integer.value = 0;
                break;
            default:
                break;
        }
    } else if (node->node_operation.right->kind == NODE_INTEGER &&
               node->node_operation.right->node_integer.value == 0) {
        switch (node->node_operation.op) {
            case OP_ADD:
            case OP_SUB:
                AST_node_free(node->node_operation.right);
                *node = *node->node_operation.left;
                break;
            case OP_MUL:
                AST_node_free(node->node_operation.left);
                AST_node_free(node->node_operation.right);
                node->kind = NODE_INTEGER;
                node->node_integer.value = 0;
                break;
            case OP_DIV:
                fprintf(stderr, "Compiler error: divide by 0.\n");
                exit(1);
            default:
                break;
        }
    }
}

void AST_optimize_if(struct AST_node* node) {
    AST_optimize(node->node_if.condition);
    AST_optimize(node->node_if.if_true);
    AST_optimize(node->node_if.if_false);
    if(node->node_if.condition->kind == NODE_INTEGER) {
        int value = node->node_if.condition->node_integer.value;
        AST_node_free(node->node_if.condition);
        if(value == 0) {
            AST_node_free(node->node_if.if_true);
            *node = *node->node_if.if_false;
        } else {
            AST_node_free(node->node_if.if_false);
            *node = *node->node_if.if_true;
        }
    }
}

void AST_optimize(struct AST_node *node) {
    switch (node->kind) {
        case NODE_RETURN:
            AST_optimize(node->node_return.expression);
            break;
        case NODE_OPERATION:
            AST_optimize_operation(node);
            break;
        case NODE_ASSIGNEMENT:
            AST_optimize(node->node_assignement.value);
            break;
        case NODE_BODY:
            for (unsigned int i = 0; i < node->node_body.children_count; i++) {
                AST_optimize(node->node_body.children[i]);
            }
            break;
        case NODE_CALL:
            for (unsigned int i = 0; i < node->node_call.parameters->parameters_count; i++) {
                AST_optimize(node->node_call.parameters->parameters[i]);
            }
            break;
        case NODE_IF:
            AST_optimize_if(node);
            break;
        case NODE_DECLARATION:
            if(node->node_declaration.symbol->usage_count == 1) {
                node->kind = NODE_NOP;
            }
            break;
        case NODE_NOP:
        case NODE_INTEGER:
        case NODE_SYMBOL:
            break;
    }
}