GRM=compilo.y
LEX=compilo.l
BIN=compilo

CC=gcc
CFLAGS=-Wall -g

OBJ=y.tab.o lex.yy.o main.o symbol_table.o function_table.o compil.o type.o program.o

all: $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

y.tab.c: $(GRM)
	-yacc -d -t -v $<

lex.yy.c: $(LEX)
	flex $<

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

clean:
	rm $(OBJ) y.tab.c y.tab.h lex.yy.c

