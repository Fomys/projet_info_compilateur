GRM=compilo.y
LEX=compilo.l
BIN=compilo

CC=gcc
CFLAGS=-Wall -g

OBJ=y.tab.o lex.yy.o main.o ast.o ast_alloc.o ast_optimizer.o symbol_table.o assembler.o memory_manager.o instruction_array.o

all: $(BIN)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

y.tab.c: $(GRM)
	yacc -d -t -v $<

lex.yy.c: $(LEX)
	flex $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) y.tab.c y.tab.h lex.yy.c

