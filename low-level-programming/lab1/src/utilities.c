#include "utilities.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

    db->firstTable = (Node *)firstTable;

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

Row* getFirstRow(Database *db, const char *tableName) {
    Table *table = getTableByName(db, tableName);
    if (table == NULL || table->metadata.firstRowOffset == -1) {
        return NULL;
    }

    Row *firstRow = deserializeRow(db, table->metadata.firstRowOffset);
    return firstRow;
}

Row* addEmptyRow(Database *db, const char *tableName) {
    if (db == NULL || tableName == NULL) {
        fprintf(stderr, "Error: Invalid parameters for addEmptyRow.\n");
        return NULL;
    }

    // Get the specified table
    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return NULL;
    }

    // Allocate memory for the new row
    Row *newRow = (Row *)malloc(sizeof(Row));
    if (newRow == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Row.\n");
        return NULL;
    }

    // Initialize the new row node
    fseek(db->dbConnection, 0, SEEK_END);
    newRow->node.offset = ftell(db->dbConnection);
    newRow->node.prevOffset = -1; // New row is at the beginning
    newRow->node.nextOffset = table->metadata.firstRowOffset; // Next is the current first row
    newRow->node.prevNode = NULL;
    newRow->node.nextNode = NULL;
    newRow->firstData = NULL; // No data in the new row yet

    // Initialize row metadata
    newRow->metadata.firstDataOffset = -1;

    // Serialize the new row (this should update newRow->node.offset)
    serializeRow(db, newRow);

    // Get the current first row and update its previous link
    Row *currentFirstRow = getFirstRow(db, tableName);
    if (currentFirstRow != NULL) {
        currentFirstRow->node.prevOffset = newRow->node.offset;

        // Link newRow and currentFirstRow
        newRow->node.nextNode = (Node *) currentFirstRow;
        currentFirstRow->node.prevNode = (Node *) newRow;

        serializeRow(db, currentFirstRow);
    }

    // Update the table's firstRowOffset and increment the record count
    table->metadata.firstRowOffset = newRow->node.offset;
    table->metadata.recordCount++;
    table->firstRow = (Node *) newRow;

    // Serialize the updated table metadata
    serializeTable(db, table);

    return newRow;
}

Data* getFirstData(Database *db, Row* row) {
    if (db == NULL || row == NULL || row->metadata.firstDataOffset == -1) {
        return NULL;
    }

    Data *firstData = deserializeData(db, row->metadata.firstDataOffset);
    return firstData;
}

Row* addData(Database *db, Row* row, Data *data) {
    if (db == NULL || row == NULL || data == NULL) {
        fprintf(stderr, "Error: Invalid parameters for addData.\n");
        return NULL;
    }

    // Allocate memory for the new data
    Data *newData = (Data *)malloc(sizeof(Data));
    if (newData == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Data.\n");
        return NULL;
    }

    // Copy data properties
    newData->type = data->type;
    newData->dataSize = data->dataSize;
    // Copy the value based on the data type
    switch (data->type) {
        case STRING:
            newData->dataSize = strlen(data->value.strValue);
            newData->value.strValue = strdup(data->value.strValue);
            break;
        case BOOLEAN:
            newData->value.boolValue = data->value.boolValue;
            break;
        case INTEGER:
            newData->value.intValue = data->value.intValue;
            break;
        case FLOAT:
            newData->value.floatValue = data->value.floatValue;
            break;
    }

    // Set the offset for the new data
    fseek(db->dbConnection, 0, SEEK_END);
    newData->node.offset = ftell(db->dbConnection);
    newData->node.prevOffset = -1; // New data is at the beginning
    newData->node.nextOffset = row->metadata.firstDataOffset; // Next is the current first data
    newData->node.prevNode = NULL;
    newData->node.nextNode = NULL;
    // Get the current first data and update its previous link
    Data *currentFirstData = getFirstData(db, row);
    if (currentFirstData != NULL) {
        currentFirstData->node.prevOffset = data->node.offset;

        // Link data and currentFirstData
        data->node.nextNode = (Node *) currentFirstData;
        currentFirstData->node.prevNode = (Node *)data;

        serializeData(db, currentFirstData);
    }

    // Serialize the new data
    serializeData(db, newData);

    // Update the row's firstDataOffset and link the new data
    row->metadata.firstDataOffset = newData->node.offset;
    row->firstData = (Node *)newData;

    // Serialize the updated row metadata
    serializeRow(db, row);

    return row;
}

Column* getNextColumn(Database *db, Column *currentColumn) {
    if (db == NULL || currentColumn == NULL || currentColumn->node.nextOffset == -1) {
        return NULL;
    }

    Column *nextColumn = deserializeColumn(db, currentColumn->node.nextOffset);
    return nextColumn;
}

Data* getDataByIndex(Database *db, const char *tableName, Row* row, int index) {
    if (db == NULL || tableName == NULL || row == NULL || index < 0) {
        return NULL;
    }

    // Get the specified table to find the number of columns
    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        return NULL;
    }

    Data *currentData = getFirstData(db, row);
    int currentIndex = 0;

    // Calculate the target index based on the prepend operation
    int targetIndex = table->metadata.columnCount - 1 - index;

    while (currentData != NULL && currentIndex < targetIndex) {
        Data *nextData = deserializeData(db, currentData->node.nextOffset);
        free(currentData); // Clean up the current data
        currentData = nextData;
        currentIndex++;
    }

    return currentIndex == targetIndex ? currentData : NULL;
}

Row* getPrevRow(Database *db, Row* currentRow) {
    if (db == NULL || currentRow == NULL || currentRow->node.prevOffset == -1) {
        return NULL;
    }

    Row *prevRow = deserializeRow(db, currentRow->node.prevOffset);
    return prevRow;
}

Row* getNextRow(Database *db, Row* currentRow) {
    if (db == NULL || currentRow == NULL || currentRow->node.nextOffset == -1) {
        return NULL;
    }

    Row *nextRow = deserializeRow(db, currentRow->node.nextOffset);
    return nextRow;
}

void deleteRow(Database *db, const char *tableName, Row* rowToDelete) {
    if (db == NULL || tableName == NULL || rowToDelete == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deleteRow.\n");
        return;
    }

    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return;
    }

    Row *prevRow = getPrevRow(db, rowToDelete);
    Row *nextRow = getNextRow(db, rowToDelete);

    if (prevRow != NULL) {
        prevRow->node.nextOffset = rowToDelete->node.nextOffset;
        serializeRow(db, prevRow);
        free(prevRow);
    } else {
        table->metadata.firstRowOffset = rowToDelete->node.nextOffset;
    }

    if (nextRow != NULL) {
        nextRow->node.prevOffset = rowToDelete->node.prevOffset;
        serializeRow(db, nextRow);
        free(nextRow);
    }

    if (table->metadata.recordCount > 0) {
        table->metadata.recordCount--;
    }

    serializeTable(db, table);
}

int getColumnCount(Database* db, const char* tableName) {
    Table* table = getTableByName(db, tableName);
    if (table != NULL) {
        return table->metadata.columnCount;
    }
    return -1;
}

Column* getColumnByIndex(Database* db, Table* table, int index) {
    if (db == NULL || table == NULL || index < 0 || index >= table->metadata.columnCount) {
        return NULL;
    }

    // Columns are prepended, so the last column has index 0, second last has index 1, and so on.
    int targetIndex = table->metadata.columnCount - 1 - index;

    Column* currentColumn = getFirstColumn(db, table->metadata.tableName.value);
    for (int i = 0; currentColumn != NULL && i < targetIndex; ++i) {
        Column* nextColumn = getNextColumn(db, currentColumn);
        currentColumn = nextColumn;
    }

    return currentColumn; // This will be NULL if index is out of range
}

int isColumnExists(Database* db, const char* tableName, const char* columnName) {
    if (db == NULL || tableName == NULL || columnName == NULL) {
        return false;
    }

    Table* table = getTableByName(db, tableName);
    if (table == NULL) {
        return false;
    }

    for (int i = 0; i < table->metadata.columnCount; i++) {
        Column* column = getColumnByIndex(db, table, i);
        if (column != NULL && strcmp(column->metadata.columnName.value, columnName) == 0) {
            return table->metadata.columnCount - i - 1; 
        }
    }

    return 0;
}









