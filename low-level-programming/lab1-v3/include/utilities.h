#ifndef UTILITIES_H
#define UTILITIES_H

#include "types.h"

Table* getFirstTable(Database *db);

Table* getTableByName(Database *db, const char *tableName);

Column* getFirstColumn(Database *db, const char *tableName);


#endif // UTILITIES_H