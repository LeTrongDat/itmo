#ifndef UTILITIES_H
#define UTILITIES_H

#include "db.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to write individual data items to file
void writeDataToFile(FILE *file, const Data *data);

void writeDatabaseToFile(const Database *db);

TableRow *readRow(FILE *file, TableColumn *columns, int numColumns);

Table *readTable(FILE *file);

// Helper function to free a table row
void freeTableRow(TableRow *row);

// Helper function to free a table
void freeTable(Table *table);

Table *getTable(Database *db, const char *tableName);

#endif