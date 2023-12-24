%{
    #include "../include/types.h"
    #include "parser.h" 
    extern FILE* yyin;
%}

%%

"CREATE"                      { return CREATE; }
"TABLE"                       { return TABLE; }
"SELECT"                      { return SELECT; }
"FROM"                        { return FROM; }
"WHERE"                       { return WHERE; }
"JOIN"                        { return JOIN; }
"ON"                          { return ON; }
"UPDATE"                      { return UPDATE; }
"SET"                         { return SET; }
"INSERT"                      { return INSERT; }
"INTO"                        { return INTO; }
"VALUES"                      { return VALUES; }
"DELETE"                      { return DELETE; }
"PRIMARY KEY"                 { return PRIMARY_KEY; }
"FOREIGN KEY"                 { return FOREIGN_KEY; }
"INT"                         { return INT; }
"VARCHAR"                     { return VARCHAR; }
"CHAR"                        { return CHAR; }
"BOOLEAN"                     { return BOOLEAN; }
"FLOAT"                       { return FLOAT; }
"DOUBLE"                      { return DOUBLE; }
"("                           { return LEFT_PAREN; }
")"                           { return RIGHT_PAREN; }
","                           { return COMMA; }
";"                           { return SEMICOLON; }
">="                          { return GE; }
"!="                          { return NE; }
"<="                          { return LE; }  
"="                           { return EQUAL; }
"+"                           { return PLUS; }
"-"                           { return MINUS; }
"*"                           { return MULTIPLY; }
"/"                           { return DIVIDE; }
">"                           { return GT; }
"<"                           { return LT; }      
"AND"                         { return AND; }
"OR"                          { return OR; }
[a-zA-Z_][a-zA-Z0-9_\.]*      { yylval.strVal = strdup(yytext); return IDENTIFIER; }
[0-9]+                        { yylval.strVal = strdup(yytext); return NUMBER; }


[ \t\n]+                      { }

%%

int yywrap(void) {
    return 1;
}
