#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "../../lab2/include/types.h"

Database* openDatabase(const char *databaseName);
Database* createDatabase(const char *databaseName);
void closeDatabase(Database *db);
void dropDatabase(const char *databaseName);

void createTable(Database *db, const char *tableName);
void addColumn(Database *db, const char *tableName, ColumnMetadata columnMetadata);
void addRow(Database *db, const char *tableName, Data *data);

typedef struct PredicateContext {
    ASTNode *ast;
    char* tableName;
} PredicateContext;

typedef bool (*PredicateFunction)(Database *db, Row* row, PredicateContext *context);

RowNode* queryRows(Database *db, const char *tableName, PredicateFunction predicate, PredicateContext* context);
void updateRows(Database *db, const char *tableName, PredicateFunction predicate, Data *newData, PredicateContext* context);
void deleteRows(Database *db, const char *tableName, PredicateFunction predicate, PredicateContext* context);


#endif // OPERATIONS_H
