%{
#include "parser.tab.h"
%}

%%

[ \t\n]+             { /* Bỏ qua khoảng trắng */ }

"SELECT"             { return SELECT; }
"FROM"               { return FROM; }
"WHERE"              { return WHERE; }
"CREATE"             { return CREATE; }
"TABLE"              { return TABLE; }
"UPDATE"             { return UPDATE; }
"SET"                { return SET; }
"DELETE"             { return DELETE; }
"JOIN"               { return JOIN; }
"ON"                 { return ON; }
"INNER"              { return INNER; }
"LEFT"               { return LEFT; }
"RIGHT"              { return RIGHT; }
"FULL"               { return FULL; }
"AND"                { return AND; }
"OR"                 { return OR; }
"IN"                 { return IN; }
"NOT"                { return NOT; }
"EXISTS"             { return EXISTS; }
"BOOLEAN"            { return BOOLEAN; }
"="                  { return EQ; }
"!="                 { return NE; }
"<"                  { return LT; }
"<="                 { return LE; }
">"                  { return GT; }
">="                 { return GE; }
"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return MULTIPLY; }
"/"                  { return DIVIDE; }
"("                  { return LPAREN; }
")"                  { return RPAREN; }
","                  { return COMMA; }
[a-zA-Z_][a-zA-Z0-9_]* { return IDENTIFIER; }
[0-9]+               { return NUMBER; }
\"[^\"]*\"           { return STRING; }

%%

int yywrap() {
    return 1;
}
