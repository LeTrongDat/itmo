#include "../include/request_handler.h"
#include "../../../lab2/build/lexer.h"
#include "../../../lab2/include/types.h"

#include <string.h>
#include <stdio.h>

extern YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse();
extern ASTNode *root;

char *err;

void yyerror(char *s);

void yyerror(char *s) {
    err = s;
}

int createTableRequest(ASTNode *root, Database__Response *response, Database *db) {
    createTable(db, root->value);
    ASTNode *columnList = (ASTNode *)malloc(sizeof(ASTNode));
    if (!columnList) {
        return 0;
    }
    for (int i = 0; i < root->child_count; i++) {
        if (root->children[i] && root->children[i]->type == NODE_TYPE_COLUMN_DEF_LIST) {
            memcpy(columnList, root->children[i], sizeof(ASTNode));
            break;    
        }
    } 

    for (int i = 0; i < columnList->child_count; i++) {
        if (!columnList->children[i]) {
            continue;
        }
        ASTNode *column = (ASTNode *)malloc(sizeof(ASTNode));
        if (!column) {
            continue;
        }

        memcpy(column, columnList->children[i], sizeof(ASTNode));
        ColumnMetadata columnMetadata; 
        columnMetadata.columnName.value = column->value;

        if (strcmp(column->children[0]->tokenName, "INT") == 0) {
            columnMetadata.dataType = INTEGER;
        } else if (strcmp(column->children[0]->tokenName, "VARCHAR") == 0 || 
                strcmp(column->children[0]->tokenName, "CHAR") == 0) {
            columnMetadata.dataType = STRING;
        } else if (strcmp(column->children[0]->tokenName, "BOOLEAN") == 0) {
            columnMetadata.dataType = BOOLEAN;
        } else if (strcmp(column->children[0]->tokenName, "FLOAT") == 0 || 
                strcmp(column->children[0]->tokenName, "DOUBLE") == 0) {
            columnMetadata.dataType = FLOAT;
        }

        addColumn(db, root->value, columnMetadata);
    }
    response->message = toJSONTable(db, root->value);
    response->success = 1;
    return 1;
}

int selectRequest(ASTNode *root, Database__Response *response, Database *db) {
    return 1;
}


void handleRequest(const Database__Request *request, Database__Response *response, Database *db) {
    root = NULL;
    YY_BUFFER_STATE buffer = yy_scan_string(request->query);
    yyparse();
    yy_delete_buffer(buffer);

    if (!root) {
        response->message = err;
        response->success = 0;
        return;
    }

    printf("%s\n", toString(root, 0));

    switch (root->type) {
    case NODE_TYPE_CREATE_TABLE:
        createTableRequest(root, response, db);
        break;
    case NODE_TYPE_SELECT:
        selectRequest(root, response, db);
        break;
    case NODE_TYPE_INSERT:
        break;
    case NODE_TYPE_UPDATE:
        break;
    case NODE_TYPE_DELETE:
        break;
    default:
        response->message = strdup("Invalid query");
        response->success = 0;
    }
    return;
}