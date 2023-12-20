# BNF Notes

```bnf
<create-statement> ::= CREATE TABLE <table-name> '(' <column-definition-list> ')'
<column-definition-list> ::= <column-definition> | <column-definition> ',' <column-definition-list>
<column-definition> ::= <column-name> <data-type>

<select-statement> ::= SELECT <column-list> FROM <table-expression> <where-clause> <group-by-clause> <having-clause> <order-by-clause>
<table-expression> ::= <table-name>
                      | <table-name> <join-type> JOIN <table-expression> ON <condition>
                      | '(' <select-statement> ')' AS <alias>
<group-by-clause> ::= GROUP BY <column-list> | ε
<having-clause> ::= HAVING <condition> | ε
<order-by-clause> ::= ORDER BY <order-list> | ε

<update-statement> ::= UPDATE <table-name> SET <assignment-list> <where-clause>
<assignment-list> ::= <assignment> | <assignment> ',' <assignment-list>
<assignment> ::= <column-name> '=' <expression>

<delete-statement> ::= DELETE FROM <table-name> <where-clause>

<condition> ::= <expression> <comparator> <expression>
              | <expression> IN '(' <select-statement> ')'
              | <expression> NOT IN '(' <select-statement> ')'
              | <expression> EXISTS '(' <select-statement> ')'
              | <expression> <logical-operator> <expression>
<expression> ::= <column-name> 
                | <literal> 
                | <expression> <arithmetic-operator> <expression>
                | '(' <select-statement> ')'

<column-list> ::= '*' | <column-name> | <column-name> ',' <column-list>
<order-list> ::= <order-item> | <order-item> ',' <order-list>
<order-item> ::= <column-name> ASC | <column-name> DESC
<comparator> ::= '=' | '!=' | '<' | '<=' | '>' | '>='
<logical-operator> ::= AND | OR
<join-type> ::= INNER | LEFT | RIGHT | FULL
<literal> ::= <number> | <string> | <boolean>
<arithmetic-operator> ::= '+' | '-' | '*' | '/'
<alias> ::= IDENTIFIER
```