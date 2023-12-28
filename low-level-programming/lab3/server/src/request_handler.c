#include "../include/request_handler.h"
#include <string.h>
#include <stdio.h>
#include "../../../lab2/build/lexer.h"
#include "../../../lab2/include/types.h"

extern YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse();
extern ASTNode *root;

void yyerror(char *s);

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

void handle_request(const Database__Request *request, Database__Response *response, Database *db) {
    YY_BUFFER_STATE buffer = yy_scan_string(request->query);
    yyparse();
    yy_delete_buffer(buffer);
    printf("%s\n", root->tokenName);
    response->message = strdup("Request processed successfully");
    response->success = 1;
}