#ifndef EXPRESSION_H
#define EXPRESSION_H

struct expression {};

struct expression_stack {
    unsigned int expression_count;
    struct expression * expressions;
};

struct expression * expression_alloc();
void expression_free(struct expression * expression);

struct expression_stack * expression_stack_alloc();
void expression_stack_free(struct expression_stack * stack);

#endif //EXPRESSION_H
