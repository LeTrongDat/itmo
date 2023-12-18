#ifndef UTILITIES_H
#define UTILITIES_H

#include "types.h"

Table* getFirstTable(Database *db);

Table* getTableByName(Database *db, const char *tableName);

Column* getFirstColumn(Database *db, const char *tableName);

Column* getNextColumn(Database *db, Column *currentColumn);

Row* getFirstRow(Database *db, const char *tableName);

Row* addEmptyRow(Database *db, const char *tableName);

Row* addData(Database *db, Row* row, Data *data);

Data* getFirstData(Database *db, Row* row);

Data* getDataByIndex(Database *db, const char *tableName, Row* row, int index);

Row* getPrevRow(Database *db, Row* currentRow);

Row* getNextRow(Database *db, Row* currentRow);

void deleteRow(Database *db, const char *tableName, Row* rowToDelete);

#endif // UTILITIES_H