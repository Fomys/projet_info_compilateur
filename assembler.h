#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "ast.h"
#include "symbol_table.h"
#include "memory_manager.h"

struct instruction_array* assemble(struct AST_node*);
struct symbol* assemble_node(struct AST_node* node, struct symbol_table* symbol_table, struct instruction_array* array);
void associate_registers(struct memory_manager* manager, struct instruction_array* array);
void resolve_functions(struct instruction_array* array, struct memory_manager* manager);

#endif //ASSEMBLER_H
