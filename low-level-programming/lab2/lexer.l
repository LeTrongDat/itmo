%{
    #include "ast.h"
#include "y.tab.h"
extern FILE *yyin;
%}

%%

CREATE { return CREATE; }
TABLE  { return TABLE; }
[0-9]+ { yylval.numVal = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strVal = strdup(yytext); return IDENTIFIER; }
\( { return LPAREN; }
\) { return RPAREN; }
,  { return COMMA; }
;  { return SEMICOLON; }
\n { /* ignore newline */ }
[ \t]+ { /* ignore whitespace */ }
.   { /* catch all for other characters */ }

%%

int yywrap() {
    return 1;
}