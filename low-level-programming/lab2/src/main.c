#include <stdio.h>
#include "../include/types.h"

void yyerror(char *s);

extern FILE *yyin;
extern int yyparse();
extern ASTNode *root;

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror(argv[1]);
        return 1;
    }

    yyin = file;
    yyparse();

    printf("%s\n", toString(root, 0));
    printf("-------------------------------------------------\n");

    fclose(file);
    return 0;
}