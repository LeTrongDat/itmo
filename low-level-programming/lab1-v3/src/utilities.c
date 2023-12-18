#include "utilities.h"

Table* getFirstTable(Database *db) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid database for retrieving first table.\n");
        return NULL;
    }

    if (db->metadata.firstTableOffset == -1) {
        db->firstTable = NULL; // No first table
        return NULL;
    }

    Table *firstTable = deserializeTable(db, db->metadata.firstTableOffset);
    if (firstTable == NULL) {
        fprintf(stderr, "Error: Failed to deserialize the first table.\n");
        db->firstTable = NULL;
        return NULL;
    }

    db->firstTable = firstTable;

    return firstTable;
}

Table* getTableByName(Database *db, const char *tableName) {
    if (db == NULL || tableName == NULL) {
        return NULL;
    }

    Table *currentTable = getFirstTable(db);

    while (currentTable != NULL) {
        if (strcmp(currentTable->metadata.tableName.value, tableName) == 0) {
            return currentTable;
        }

        long nextOffset = currentTable->node.nextOffset;
        if (nextOffset == -1) {
            break;
        }
        currentTable = deserializeTable(db, nextOffset);
    }

    return NULL;
}

Column* getFirstColumn(Database *db, const char *tableName) {
    Table *table = getTableByName(db, tableName);
    if (table == NULL || table->metadata.firstColumnOffset == -1) {
        return NULL;
    }

    Column *firstColumn = deserializeColumn(db, table->metadata.firstColumnOffset);
    return firstColumn;
}
