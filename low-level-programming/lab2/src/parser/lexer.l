%{
#include "../include/types.h"
#include "parser.h"
extern FILE *yyin;
%}

%%

CREATE                      { return CREATE; }
TABLE                       { return TABLE; }
[0-9]+                      { yylval.numVal = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.strVal = strdup(yytext); return IDENTIFIER; }
\(                          { return LPAREN; }
\)                          { return RPAREN; }
,                           { return COMMA; }
;                           { return SEMICOLON; }
\n                         
[ \t]+                     
.                           { return 0; }

%%

int yywrap() {
    return 1;
}
