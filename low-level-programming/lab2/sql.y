%{
#include <stdio.h>
#include <stdlib.h>
%}

%token IDENTIFIER NUMBER STRING
%token SELECT FROM WHERE CREATE TABLE UPDATE SET DELETE
%token JOIN ON INNER LEFT RIGHT FULL AND OR IN NOT EXISTS BOOLEAN
%token EQ NE LT LE GT GE
%token PLUS MINUS MULTIPLY DIVIDE
%token LPAREN RPAREN
%token COMMA 

%left MULTIPLY DIVIDE
%left PLUS MINUS
%right UMINUS

%%

sql_statement:
    create_statement
    | select_statement
    | update_statement
    | delete_statement
    ;

create_statement:
    CREATE TABLE IDENTIFIER LPAREN column_definition_list RPAREN
    ;

column_definition_list:
    column_definition
    | column_definition COMMA column_definition_list
    ;

column_definition:
    IDENTIFIER data_type
    ;

data_type:
    NUMBER
    | STRING
    | BOOLEAN
    ;

select_statement:
    SELECT column_list FROM table_expression where_clause
    ;

update_statement:
    UPDATE IDENTIFIER SET assignment_list where_clause
    ;

assignment_list:
    assignment
    | assignment COMMA assignment_list
    ;

assignment:
    IDENTIFIER EQ expression
    ;

delete_statement:
    DELETE FROM IDENTIFIER where_clause
    ;

table_expression:
    IDENTIFIER
    | IDENTIFIER join_type JOIN table_expression ON condition
    | LPAREN select_statement RPAREN
    ;

join_type:
    INNER
    | LEFT
    | RIGHT
    | FULL
    ;

where_clause:
    WHERE condition
    ;

condition:
    expression comparator expression
    | expression IN LPAREN select_statement RPAREN
    | expression NOT IN LPAREN select_statement RPAREN
    | expression EXISTS LPAREN select_statement RPAREN
    | expression logical_operator expression
    ;

expression:
    IDENTIFIER
    | literal
    | expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | MINUS expression %prec UMINUS
    | LPAREN expression RPAREN
    ;

column_list:
    '*' 
    | IDENTIFIER
    | IDENTIFIER COMMA column_list
    ;

comparator:
    EQ
    | NE
    | LT
    | LE
    | GT
    | GE
    ;

logical_operator:
    AND
    | OR
    ;

literal:
    NUMBER
    | STRING
    | BOOLEAN
    ;

%%

int main(int argc, char **argv) {
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
