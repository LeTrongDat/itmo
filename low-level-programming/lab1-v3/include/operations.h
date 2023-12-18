#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

Database* openDatabase(const char *databaseName);
Database* createDatabase(const char *databaseName);
void closeDatabase(Database *db);
void dropDatabase(const char *databaseName);

void createTable(Database *db, const char *tableName);
void addColumn(Database *db, const char *tableName, ColumnMetadata columnMetadata);

#endif // OPERATIONS_H
