%{
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
void yyerror(char *s);
extern void printAST(CreateTable* createTable);
extern FILE *yyin;
%}

%token CREATE TABLE NUMBER
%token LPAREN RPAREN COMMA SEMICOLON
%token <strVal> IDENTIFIER

%union {
    char* strVal;
    int numVal;
    Column* colVal;
    CreateTable* createTableVal;
}

%type <createTableVal> command
%type <createTableVal> create_table
%type <colVal> column_def
%type <colVal> column_defs

%%

commands:
    | commands command SEMICOLON { printAST($2); }
    ;

command:
    create_table { $$ = $1; }
    ;

create_table:
    CREATE TABLE IDENTIFIER LPAREN column_defs RPAREN {
        $$ = malloc(sizeof(CreateTable));
        $$->tableName = $3;
        $$->columns = $5;
    }
    ;

column_defs:
    column_def { $$ = $1; }
    | column_defs COMMA column_def {
        Column* col = $1;
        while(col->next) col = col->next;
        col->next = $3;
        $$ = $1;
    }
    ;

column_def:
    IDENTIFIER {
        $$ = malloc(sizeof(Column));
        $$->name = $1;
        $$->next = NULL;
    }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(int argc, char **argv) {
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
    fclose(file);

    return 0;
}
