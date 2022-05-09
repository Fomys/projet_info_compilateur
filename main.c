#include <stdio.h>

extern FILE *yyin;
extern FILE *yyout;

void yyparse();

void yyerror(char *s) {
    printf("%s\n", s);
}

int main(int argc, char ** argv) {
    if(argc >= 3) {
        yyin = fopen(argv[1], "r");
        yyout = fopen(argv[2], "w");
        yyparse();
    } else {
        yyout = stdout;
        yyparse();
    }
    return 0;
}

