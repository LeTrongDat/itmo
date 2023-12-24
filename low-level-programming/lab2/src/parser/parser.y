%{
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
void yyerror(char *s);
extern FILE *yyin;
%}

%union {
    ASTNode *ast_node;
    char *strVal;
    int intVal;
}

%token INT VARCHAR CHAR BOOLEAN FLOAT DOUBLE
%token IDENTIFIER NUMBER STRING
%token CREATE TABLE SELECT FROM WHERE JOIN ON UPDATE SET INSERT INTO DELETE
%token PRIMARY_KEY FOREIGN_KEY REFERENCES
%token LEFT_PAREN RIGHT_PAREN COMMA EQUAL PLUS MINUS MULTIPLY DIVIDE SEMICOLON AND OR VALUES
%token GT LT GE LE NE

%left NE GT LT GE LE 
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left AND
%left OR

%type <ast_node> stmt select_stmt from_clause where_clause condition join_clause expression table_name column_name
%type <ast_node> create_table_stmt update_stmt insert_stmt delete_stmt column_def_list column_def data_type column_constraint
%type <ast_node> column_list value_list select_list

%type <strVal> IDENTIFIER STRING NUMBER

%%

stmt: select_stmt SEMICOLON { printAST($1, 0); printf("-------------------------------------------------\n"); }
    | create_table_stmt SEMICOLON { printAST($1, 0); printf("-------------------------------------------------\n"); }
    | update_stmt SEMICOLON { printAST($1, 0); printf("-------------------------------------------------\n"); }
    | insert_stmt SEMICOLON { printAST($1, 0); printf("-------------------------------------------------\n"); }
    | delete_stmt SEMICOLON { printAST($1, 0); printf("-------------------------------------------------\n"); }
    ;

select_stmt: SELECT select_list from_clause join_clause where_clause {
                $$ = createNode(NODE_TYPE_SELECT, "SELECT", NULL, 4, $2, $3, $4, $5);
             }
    ;

select_list: column_name {
                $$ = createNode(NODE_TYPE_COLUMN_LIST, "COLUMN_LIST", NULL, 1, $1);
             }
    | select_list COMMA column_name {
                mergeNodes($1, createNode(NODE_TYPE_COLUMN_LIST, "COLUMN_LIST", NULL, 1, $3));
                $$ = $1;
             }
    ;

column_name: IDENTIFIER {
                $$ = createNode(NODE_TYPE_COLUMN, "COLUMN", $1, 0);
             }
    ;

from_clause: FROM table_name {
                $$ = createNode(NODE_TYPE_FROM, "FROM", NULL, 1, $2);
             }
    ;

join_clause: { $$ = NULL; }
    | JOIN table_name ON condition {
                $$ = createNode(NODE_TYPE_JOIN, "JOIN", NULL, 2, $2, $4);
             }
    ;

where_clause: { $$ = NULL; }
    | WHERE condition {
                $$ = createNode(NODE_TYPE_WHERE, "WHERE", NULL, 1, $2);
             }
    ;

condition: column_name EQUAL expression {
                $$ = createNode(NODE_TYPE_CONDITION, "=", NULL, 2, $1, $3);
             }
    | column_name NE expression {
        $$ = createNode(NODE_TYPE_NE, "NE", NULL, 2, $1, $3);
    }
    | column_name GT expression {
        $$ = createNode(NODE_TYPE_GT, "GT", NULL, 2, $1, $3);
    }
    | column_name LT expression {
        $$ = createNode(NODE_TYPE_LT, "LT", NULL, 2, $1, $3);
    }
    | column_name GE expression {
        $$ = createNode(NODE_TYPE_GE, "GE", NULL, 2, $1, $3);
    }
    | column_name LE expression {
        $$ = createNode(NODE_TYPE_LE, "LE", NULL, 2, $1, $3);
    }
    | condition AND condition {
                $$ = createNode(NODE_TYPE_CONDITION, "AND", NULL, 2, $1, $3);
             }
    | condition OR condition {
                $$ = createNode(NODE_TYPE_CONDITION, "OR", NULL, 2, $1, $3);
             }
    ;

expression: expression PLUS expression {
                $$ = createNode(NODE_TYPE_EXPRESSION, "ADD", NULL, 2, $1, $3);
             }
    | expression MINUS expression {
                $$ = createNode(NODE_TYPE_EXPRESSION, "SUBTRACT", NULL, 2, $1, $3);
             }
    | expression MULTIPLY expression {
                $$ = createNode(NODE_TYPE_EXPRESSION, "MULTIPLY", NULL, 2, $1, $3);
             }
    | expression DIVIDE expression {
                $$ = createNode(NODE_TYPE_EXPRESSION, "DIVIDE", NULL, 2, $1, $3);
             }

    | LEFT_PAREN expression RIGHT_PAREN {
                $$ = $2;
             }
    | NUMBER {
                $$ = createNode(NODE_TYPE_LITERAL, "LITERAL", $1, 0);
             }
    | IDENTIFIER {
                $$ = createNode(NODE_TYPE_LITERAL, "LITERAL", $1, 0);
             }
    ;

table_name: IDENTIFIER {
                $$ = createNode(NODE_TYPE_TABLE, "TABLE", $1, 0);
             }
    ;

create_table_stmt: CREATE TABLE IDENTIFIER LEFT_PAREN column_def_list RIGHT_PAREN {
                        $$ = createNode(NODE_TYPE_CREATE_TABLE, "CREATE_TABLE", $3, 1, $5);
                     }
    ;

column_def_list: column_def { 
        $$ = createNode(NODE_TYPE_COLUMN_DEF_LIST, "COLUMN_DEF_LIST", NULL, 1, $1); 
    }
    | column_def_list COMMA column_def {
        mergeNodes($1, createNode(NODE_TYPE_COLUMN_DEF_LIST, "COLUMN_DEF_LIST", NULL, 1, $3));
        $$ = $1;
    }
    ;

column_def: IDENTIFIER data_type {
                $$ = createNode(NODE_TYPE_COLUMN_DEF, "COLUMN_DEF", $1, 1, $2);
             }
    ;
    
data_type: INT {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "INT", NULL, 0);
             }
    | VARCHAR LEFT_PAREN NUMBER RIGHT_PAREN {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "VARCHAR", $3, 0);
             }
    | CHAR LEFT_PAREN NUMBER RIGHT_PAREN {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "CHAR", $3, 0);
             }
    | BOOLEAN {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "BOOLEAN", NULL, 0);
             }
    | FLOAT {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "FLOAT", NULL, 0);
             }
    | DOUBLE {
                $$ = createNode(NODE_TYPE_DATA_TYPE, "DOUBLE", NULL, 0);
             }
    ;

column_constraint: { $$ = NULL; }
    | PRIMARY_KEY {
        $$ = createNode(NODE_TYPE_COLUMN_CONSTRAINT, "PRIMARY_KEY", NULL, 0);
    }
    | FOREIGN_KEY REFERENCES IDENTIFIER {
        $$ = createNode(NODE_TYPE_COLUMN_CONSTRAINT, "FOREIGN_KEY", $3, 0);
    }
    ;

update_stmt: UPDATE table_name SET column_name EQUAL expression where_clause {
                $$ = createNode(NODE_TYPE_UPDATE, "UPDATE", NULL, 4, $2, $4, $6, $7);
             }
    ;

insert_stmt: INSERT INTO table_name LEFT_PAREN column_list RIGHT_PAREN VALUES LEFT_PAREN value_list RIGHT_PAREN {
                $$ = createNode(NODE_TYPE_INSERT, "INSERT", NULL, 3, $3, $5, $9);
             }
    ;

column_list: column_name {
                $$ = createNode(NODE_TYPE_COLUMN_LIST, "COLUMN_LIST", NULL, 1, $1); 
             }
    | column_list COMMA column_name {
                mergeNodes($1, $3);
                $$ = $1;
             }
    ;

value_list: expression {
                ASTNode* valueList = createNode(NODE_TYPE_VALUE_LIST, "VALUE_LIST", NULL, 1, $1);
                $$ = valueList;
             }
    | value_list COMMA expression {
                mergeNodes($1, $3);
                $$ = $1;
             }
    ;

delete_stmt: DELETE FROM table_name where_clause {
                $$ = createNode(NODE_TYPE_DELETE, "DELETE", NULL, 2, $3, $4);
             }
    ;

%%

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

    fclose(file);
    return 0;
}