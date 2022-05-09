#ifndef AST_H
#define AST_H

#include "symbol_table.h"

struct AST_location {
    unsigned int first_line;
    unsigned int first_column;
    unsigned int last_line;
    unsigned int last_column;
};

struct AST_node;

enum AST_node_kind {
    NODE_INTEGER,
    NODE_OPERATION,
    NODE_ASSIGNEMENT,
    NODE_BODY,
    NODE_SYMBOL,
    NODE_CALL,
    NODE_IF,
    NODE_DECLARATION,
    NODE_NOP,
    NODE_RETURN
};

enum AST_op {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_GT,
    OP_LT
};

struct AST_node_integer {
    int value;
};

struct AST_node_assign {
    struct symbol *symbol;
    struct AST_node *value;
};

struct AST_node_if {
    struct AST_node *condition;
    struct AST_node *if_true;
    struct AST_node *if_false;
};

struct AST_node_operation {
    enum AST_op op;
    struct AST_node *left;
    struct AST_node *right;
};

struct AST_node_body {
    unsigned int children_count;
    struct AST_node **children;
    struct symbol_table *symbol_table;
};

struct AST_node_symbol {
    struct symbol *symbol;
};

struct call_parameters {
    struct AST_node **parameters;
    unsigned int parameters_count;
};
struct AST_node_call {
    struct symbol *symbol;
    struct call_parameters *parameters;
};

struct AST_node_declaration {
    struct symbol *symbol;
};

struct AST_node_return {
    struct AST_node* expression;
};

struct AST_node {
    enum AST_node_kind kind;
    union {
        struct AST_node_integer node_integer;
        struct AST_node_operation node_operation;
        struct AST_node_assign node_assignement;
        struct AST_node_body node_body;
        struct AST_node_symbol node_symbol;
        struct AST_node_call node_call;
        struct AST_node_if node_if;
        struct AST_node_declaration node_declaration;
        struct AST_node_return node_return;
    };
};

struct AST_node *AST_node_alloc();
void AST_node_free(struct AST_node *);
void AST_node_integer_free(struct AST_node *);
void AST_node_operation_free(struct AST_node *);
void AST_node_symbol_free(struct AST_node *);
void AST_node_assignement_free(struct AST_node *);
void AST_node_body_free(struct AST_node *);
void AST_node_call_free(struct AST_node *);
void AST_node_declaration_free(struct AST_node *);
void AST_node_return_free(struct AST_node* node);
void AST_node_if_free(struct AST_node *);
void AST_node_print(struct AST_node *);
void AST_node_body_add_child(struct AST_node *, struct AST_node *);
struct AST_node *AST_node_body_alloc();
void AST_node_integer_initialize(struct AST_node *node, int value);
struct AST_node *AST_node_assignement_from(struct symbol *, struct AST_node *);
struct AST_node *AST_node_operation_from(enum AST_op, struct AST_node *, struct AST_node *);
struct AST_node *AST_node_symbol_from(struct symbol *);
struct AST_node *AST_node_call_from(struct symbol *, struct call_parameters *);
struct AST_node *AST_node_if_else_from(struct AST_node *, struct AST_node *, struct AST_node *);
struct AST_node *AST_node_declaration_from(struct symbol *);
struct AST_node* AST_node_return_from(struct AST_node*);
void AST_optimize(struct AST_node *node);
struct call_parameters *call_parameters_alloc();
void call_parameters_free(struct call_parameters *parameters);
void call_parameters_add(struct call_parameters *, struct AST_node *);
void AST_set_symbol_table(struct AST_node *, struct symbol_table *);



#endif