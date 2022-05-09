#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include "ast.h"


struct AST_node* AST_node_alloc() {
    struct AST_node* node = malloc(sizeof(struct AST_node));
    if(node == NULL) {
        perror("Failed to allocate memory for node.\n");
        exit(errno);
    }
    return node;
}
void AST_node_free(struct AST_node* node) {
    switch (node->kind) {
        case NODE_INTEGER:
            AST_node_integer_free(node);
            break;
        case NODE_ASSIGNEMENT:
            AST_node_assignement_free(node);
            break;
        case NODE_BODY:
            AST_node_body_free(node);
            break;
        case NODE_OPERATION:
            AST_node_operation_free(node);
            break;
        case NODE_SYMBOL:
            AST_node_symbol_free(node);
            break;
        case NODE_CALL:
            AST_node_call_free(node);
            break;
        case NODE_IF:
            AST_node_if_free(node);
            break;
        case NODE_DECLARATION:
            AST_node_declaration_free(node);
            break;
        case NODE_NOP:
            break;
        case NODE_RETURN:
            AST_node_return_free(node);
            break;
    }
}
void AST_node_return_free(struct AST_node* node) {
    AST_node_free(node->node_return.expression);
    free(node);
}
void AST_node_declaration_free(struct AST_node* node) {
    free(node);
}
void AST_node_if_free(struct AST_node* node) {
    AST_node_free(node->node_if.if_false);
    AST_node_free(node->node_if.if_true);
    AST_node_free(node->node_if.condition);
    free(node);
}
void AST_node_call_free(struct AST_node* node) {
    call_parameters_free(node->node_call.parameters);
    free(node);
}
void call_parameters_free(struct call_parameters* parameters) {
    for(unsigned int i = 0; i < parameters->parameters_count; i++) {
        AST_node_free(parameters->parameters[i]);
    }
    free(parameters->parameters);
    free(parameters);
}
void AST_node_symbol_free(struct AST_node *node) {
    free(node);
}
void AST_node_integer_free(struct AST_node *node) {
    free(node);
}
void AST_node_assignement_free(struct AST_node *node) {
    AST_node_free(node->node_assignement.value);
    free(node);
}
void AST_node_body_free(struct AST_node* node) {
    for(unsigned int i = 0; i < node->node_body.children_count; i++) {
        AST_node_free(node->node_body.children[i]);
    }
    symbol_table_free(node->node_body.symbol_table);
    free(node);
}
void AST_node_operation_free(struct AST_node* node) {
    AST_node_free(node->node_operation.left);
    AST_node_free(node->node_operation.right);
    free(node);
}
struct AST_node* AST_node_body_alloc() {
    struct AST_node* node = AST_node_alloc();
    node->kind = NODE_BODY;
    node->node_body.children = malloc(0);
    node->node_body.children_count = 0;
    return node;
}

struct call_parameters* call_parameters_alloc() {
    struct call_parameters* call_parameters = malloc(sizeof(struct call_parameters));
    if (call_parameters == NULL) {
        perror("Failed to malloc for call_parameters.\n");
        exit(errno);
    }
    call_parameters->parameters = malloc(0);
    call_parameters->parameters_count = 0;
    return call_parameters;
}