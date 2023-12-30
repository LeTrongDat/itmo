#include "../include/request_handler.h"
#include "../../../lab2/build/lexer.h"
#include "../../../lab2/include/types.h"
#include "../../../lab1/include/utilities.h"

#include <string.h>
#include <stdio.h>
#include <Block.h>

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
    response->message = toJSONTable(db, root->value, NULL, NULL);
    response->success = 1;
    return 1;
}

bool convertASTToPredicate(Database *db, Row *row, PredicateContext *context) {
    ASTNode *ast = context->ast;
    if (ast == NULL) {
        return true;
    }

    if (strcmp(ast->tokenName, "WHERE") == 0) {
        PredicateContext* newCtx  = (PredicateContext*) malloc(sizeof(PredicateContext));
        if (!newCtx) {
            return false;
        }
        newCtx->ast = ast->children[0];
        newCtx->tableName = context->tableName;
        return convertASTToPredicate(db, row, newCtx);
    }

    if (strcmp(ast->tokenName, "AND") == 0) {
        PredicateContext *leftCtx, *rightCtx;
        leftCtx = (PredicateContext*) malloc(sizeof(PredicateContext));
        rightCtx = (PredicateContext*) malloc(sizeof(PredicateContext));

        leftCtx->ast = ast->children[0];
        leftCtx->tableName = context->tableName;
        rightCtx->ast = ast->children[1];
        rightCtx->tableName = context->tableName;

        return convertASTToPredicate(db, row, leftCtx) && convertASTToPredicate(db, row, rightCtx);
    }

    if (strcmp(ast->tokenName, "OR") == 0) {
        PredicateContext *leftCtx, *rightCtx;
        leftCtx = (PredicateContext*) malloc(sizeof(PredicateContext));
        rightCtx = (PredicateContext*) malloc(sizeof(PredicateContext));

        leftCtx->ast = ast->children[0];
        leftCtx->tableName = context->tableName;
        rightCtx->ast = ast->children[1];
        rightCtx->tableName = context->tableName;

        return convertASTToPredicate(db, row, leftCtx) || convertASTToPredicate(db, row, rightCtx);
    }    

    char *operator = ast->tokenName;
    char *tableName = context->tableName;
    char *columnName = ast->children[0]->value;
    int columnIndex = isColumnExists(db, tableName, columnName);
    ASTNode *expression = ast->children[1];

    Data *data = getDataByIndex(db, tableName, row, columnIndex);

    Table *table = getTableByName(db, tableName);
    Column *column = getColumnByIndex(db, table, columnIndex);
    if (column->metadata.dataType == BOOLEAN) {
        if (strcmp(operator, "=") != 0 && strcmp(operator, "!=") != 0) {
            return false;
        }

        if (strcmp(expression->value, "true") != 0 && strcmp(expression->value, "false") != 0) {
            return false;
        }

        int value = strcmp(expression->value, "true") == 0 ? 1 : 0;
        if (strcmp(operator, "=") == 0) {
            return data->value.boolValue == value;
        }
        return data->value.boolValue != value;
    } 

    if (column->metadata.dataType == STRING) {
        char *value = expression->value;
        if (strcmp(operator, "=") == 0) return strcmp(value, data->value.strValue) == 0;
        if (strcmp(operator, "NE") == 0) return strcmp(value, data->value.strValue) != 0;
        if (strcmp(operator, "GT") == 0) return strcmp(data->value.strValue, value) > 0;
        if (strcmp(operator, "LT") == 0) return strcmp(data->value.strValue, value) < 0;
        if (strcmp(operator, "GE") == 0) return strcmp(data->value.strValue, value) == 0 || strcmp(data->value.strValue, value) > 0;
        if (strcmp(operator, "LE") == 0) return strcmp(data->value.strValue, value) == 0 || strcmp(data->value.strValue, value) < 0;
        return false;
    }

    if (column->metadata.dataType == INTEGER) {
        int value = atoi(expression->value);
        if (strcmp(operator, "=") == 0) return data->value.intValue == value;
        if (strcmp(operator, "NE") == 0) return data->value.intValue != value;
        if (strcmp(operator, "GT") == 0) return data->value.intValue > value;
        if (strcmp(operator, "LT") == 0) return data->value.intValue < value;
        if (strcmp(operator, "GE") == 0) return data->value.intValue >= value;
        if (strcmp(operator, "LE") == 0) return data->value.intValue <= value;
        return false;
    }


    if (column->metadata.dataType == FLOAT) {
        int value = atoi(expression->value);
        if (strcmp(operator, "=") == 0) return data->value.floatValue == value;
        if (strcmp(operator, "NE") == 0) return data->value.floatValue != value;
        if (strcmp(operator, "GT") == 0) return data->value.floatValue > value;
        if (strcmp(operator, "LT") == 0) return data->value.floatValue < value;
        if (strcmp(operator, "GE") == 0) return data->value.floatValue >= value;
        if (strcmp(operator, "LE") == 0) return data->value.floatValue <= value;
        return false;
    }
    return false;
}

char **convertToSelectedColumns(ASTNode *columnList) {
    if (columnList == NULL || columnList->child_count == 0) {
        return NULL;
    }

    char **selectedColumns = malloc((columnList->child_count + 1) * sizeof(char *));
    if (selectedColumns == NULL) {
        return NULL;
    }

    for (int i = 0; i < columnList->child_count; i++) {
        selectedColumns[i] = strdup(columnList->children[i]->value);
    }

    selectedColumns[columnList->child_count] = NULL;

    return selectedColumns;
}

int selectRequest(ASTNode *root, Database__Response *response, Database *db) {
    ASTNode *columnList = (ASTNode *)malloc(sizeof(ASTNode));
    if (!columnList) {
        return 0;
    }
    memcpy(columnList, root->children[0], sizeof(ASTNode));

    ASTNode *from = (ASTNode *)malloc(sizeof(ASTNode));
    if (!from) {
        return 0;
    }
    memcpy(from, root->children[1], sizeof(ASTNode));

    ASTNode *join = (ASTNode *)malloc(sizeof(ASTNode));
    if (!join) {
        return 0;
    }
    memcpy(join, root->children[2], sizeof(ASTNode)); 

    ASTNode *where = (ASTNode *)malloc(sizeof(ASTNode));
    if (!where) {
        return 0;
    }
    memcpy(where, root->children[3], sizeof(ASTNode)); 

    char *tableName = from->value;
    char **selectedColumns = convertToSelectedColumns(columnList);
    PredicateContext* ctx = (PredicateContext*) malloc(sizeof(PredicateContext));
    ctx->ast = where;
    ctx->tableName = tableName;
    RowNode *rowNode = queryRows(db, tableName, convertASTToPredicate, ctx);

    response->message = toJSONTable(db, tableName, rowNode, selectedColumns);
    response->success = 1;
    return 1;
}

void convertValue(Data *data, const char *valueStr, DataType dataType) {
    switch (dataType) {
        case INTEGER:
            data->value.intValue = atoi(valueStr);  
            break;
        case FLOAT:
            data->value.floatValue = atof(valueStr); 
            break;
        case BOOLEAN:
            data->value.boolValue = (strcmp(valueStr, "true") == 0); 
            break;
        case STRING:
            data->value.strValue = strdup(valueStr);  
            break;
        default:
            break;
    }
    data->type = dataType;
}


int insertRequest(ASTNode *root, Database__Response *response, Database *db) {
    ASTNode *tableNode = root->children[0];
    ASTNode *columnList = root->children[1];
    ASTNode *valueList = root->children[2];

    const char *tableName = tableNode->value;
    Table *table = getTableByName(db, tableName);
    if (!table) {
        return 0;
    }

    int columnCount = table->metadata.columnCount;
    Data *rowData = malloc(columnCount * sizeof(Data));
    if (!rowData) {
        return 0;
    }

    for (int i = 0; i < columnCount; i++) {
        Column *column = getColumnByIndex(db, table, i);
        if (!column) {
            continue;
        }
        rowData[i].type = column->metadata.dataType;
        switch (rowData[i].type) {
            case INTEGER:
            case FLOAT:
                rowData[i].value.intValue = 0;
                break;
            case BOOLEAN:
                rowData[i].value.boolValue = 0;
                break;
            case STRING:
                rowData[i].value.strValue = strdup("");
                break;
        }
    }

    for (int i = 0; i < valueList->child_count; i++) {
        const char *columnName = columnList->children[i]->value;
        const char *valueStr = valueList->children[i]->value;

        int columnIndex = isColumnExists(db, tableName, columnName);
        if (columnIndex != -1) {
            Column *column = getColumnByIndex(db, table, columnIndex);
            if (!column) {
                continue;
            }
            convertValue(&rowData[columnIndex], valueStr, column->metadata.dataType);
        }
    }

    addRow(db, tableName, rowData);

    for (int i = 0; i < columnCount; i++) {
        if (rowData[i].type == STRING) {
            free(rowData[i].value.strValue);
        }
    }
    free(rowData);

    response->message = strdup("Insert successfully");
    response->success = true;

    return 1;
}

int deleteRequest(ASTNode *root, Database__Response *response, Database *db) {
    ASTNode *tableNode = root->children[0];
    ASTNode *where = root->children[1];

    char *tableName = tableNode->value;
    PredicateContext* ctx = (PredicateContext*) malloc(sizeof(PredicateContext));
    ctx->ast = where;
    ctx->tableName = tableName;

    deleteRows(db, tableName, convertASTToPredicate, ctx);

    response->message = strdup("Delete successfully");
    response->success = 1;
    return 1;
}

int updateRequest(ASTNode *root, Database__Response *response, Database *db) {
    ASTNode *tableNode = root->children[0];
    ASTNode *columnNode = root->children[1];
    ASTNode *expressionNode = root->children[2];
    ASTNode *whereNode = root->children[3];

    char *tableName = tableNode->value;
    char *columnName = columnNode->value;
    char *expressionValue = expressionNode->value;

    Table *table = getTableByName(db, tableName);
    if (!table) {
        return 0;
    }

    int columnIndex = isColumnExists(db, tableName, columnName);
    if (columnIndex == -1) {
        return 0;
    }

    Column *column = getColumnByIndex(db, table, columnIndex);
    if (!column) {
        return 0;
    }

    Data newData;
    newData.type = column->metadata.dataType;
    convertValue(&newData, expressionValue, newData.type);

    PredicateContext* ctx = (PredicateContext*) malloc(sizeof(PredicateContext));
    ctx->ast = whereNode;
    ctx->tableName = tableName;

    updateRows(db, tableName, convertASTToPredicate, &newData, ctx);

    if (newData.type == STRING) {
        free(newData.value.strValue);
    }

    response->message = "Update successfully";
    response->success = true;

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
        insertRequest(root, response, db);
        break;
    case NODE_TYPE_UPDATE:
        updateRequest(root, response, db);
        break;
    case NODE_TYPE_DELETE:
        deleteRequest(root, response, db);
        break;
    default:
        response->message = strdup("Invalid query");
        response->success = 0;
    }
    return;
}