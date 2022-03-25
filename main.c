#include <stdio.h>

extern FILE *yyin;

void yyerror(char *s) {
    printf("%s\n", s);
}

int main(int argc, char ** argv) {
    if(argc >= 2) {
        yyin = fopen(argv[1], "r");
        yyparse();
    } else {
        yyparse();
    }
    return 0;
}
