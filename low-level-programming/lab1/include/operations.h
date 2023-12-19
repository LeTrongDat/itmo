#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "types.h"

Database* openDatabase(const char *databaseName);
Database* createDatabase(const char *databaseName);
void closeDatabase(Database *db);
void dropDatabase(const char *databaseName);

void createTable(Database *db, const char *tableName);
void addColumn(Database *db, const char *tableName, ColumnMetadata columnMetadata);
void addRow(Database *db, const char *tableName, Data *data);

typedef bool (*PredicateFunction)(Database *db, const Row*);

typedef struct RowNode {
    Row *row;
    struct RowNode *next;
} RowNode;

RowNode* queryRows(Database *db, const char *tableName, PredicateFunction predicate);
void updateRows(Database *db, const char *tableName, PredicateFunction predicate, Data *newData);
void deleteRows(Database *db, const char *tableName, PredicateFunction predicate);


#endif // OPERATIONS_H
