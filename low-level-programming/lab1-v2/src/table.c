#include "../include/table.h"

// Serialization function for a Table object
void serializeTable(FILE *file, const Table *table) {
    if (!file || !table) return;

    if (table->fileTable.offset) fseek(file, table->fileTable.offset, SEEK_SET);

    // Write the table name
    fwrite(table->tableName, sizeof(table->tableName), 1, file);

    // Write the FileTable data (offsets)
    fwrite(&(table->fileTable), sizeof(table->fileTable), 1, file);

    // Write the metadata (number of columns and rows)
    fwrite(&(table->numColumns), sizeof(table->numColumns), 1, file);
    fwrite(&(table->numRows), sizeof(table->numRows), 1, file);

    // Additional serialization logic for the Table can be added here
}

// Deserialization function for a Table object
void deserializeTable(FILE *file, Table *table) {
    if (!file || !table) return;

    if (table->fileTable.offset) fseek(file, table->fileTable.offset, SEEK_SET);

    // Read the table name
    fread(table->tableName, sizeof(table->tableName), 1, file);

    // Read the FileTable data (offsets)
    fread(&(table->fileTable), sizeof(table->fileTable), 1, file);

    // Read the metadata (number of columns and rows)
    fread(&(table->numColumns), sizeof(table->numColumns), 1, file);
    fread(&(table->numRows), sizeof(table->numRows), 1, file);

    // Additional deserialization logic for the Table can be added here
}

// Function to get the next Table node
Table* getNextTable(FILE *file, Table *currentTable) {
    if (!currentTable) return NULL;
    if (currentTable->next) return currentTable->next;

    // Load from file if the next node is not in memory and file is provided
    if (file && currentTable->fileTable.nextTableOffset != 0) {
        Table* nextTable = malloc(sizeof(Table));
        if (!nextTable) return NULL;

        fseek(file, currentTable->fileTable.nextTableOffset, SEEK_SET);
        deserializeTable(file, nextTable);
        currentTable->next = nextTable;
        return nextTable;
    }

    return NULL;
}

// Function to get the previous Table node
Table* getPrevTable(FILE *file, Table *currentTable) {
    if (!currentTable) return NULL;
    if (currentTable->prev) return currentTable->prev;

    // Load from file if the previous node is not in memory and file is provided
    if (file && currentTable->fileTable.prevTableOffset != 0) {
        Table* prevTable = malloc(sizeof(Table));
        if (!prevTable) return NULL;

        fseek(file, currentTable->fileTable.prevTableOffset, SEEK_SET);
        deserializeTable(file, prevTable);
        currentTable->prev = prevTable;
        return prevTable;
    }

    return NULL;
}

// Function to get the last Row in the Table
Row* getLastRow(FILE *file, Table *table) {
    if (!table) return NULL;
    if (table->lastRow) return table->lastRow;

    // Load from file if the lastRow is not in memory and file is provided
    if (file && table->fileTable.lastRowOffset != 0) {
        Row* lastRow = malloc(sizeof(Row));
        if (!lastRow) return NULL;
        if (lastRow->lastData) {
        }

        fseek(file, table->fileTable.lastRowOffset, SEEK_SET);
        deserializeRow(file, lastRow);  // Assuming deserializeRow is implemented elsewhere
        table->lastRow = lastRow;
        return lastRow;
    }

    return NULL;
}

// Function to get the last Column in the Table
Column* getLastColumn(FILE *file, Table *table) {
    if (!table) return NULL;
    if (table->lastColumn) return table->lastColumn;

    // Load from file if the lastColumn is not in memory and file is provided
    if (file && table->fileTable.lastColumnOffset != 0) {
        Column* lastColumn = malloc(sizeof(Column));
        if (!lastColumn) return NULL;

        fseek(file, table->fileTable.lastColumnOffset, SEEK_SET);
        deserializeColumn(file, lastColumn);  // Assuming deserializeColumn is implemented elsewhere
        table->lastColumn = lastColumn;
        return lastColumn;
    }

    return NULL;
}

// Function to free a single Table object
void freeTable(Table* table) {
    if (!table) return;

    // Free all rows in the table
    freeRows(table->lastRow);  // Assuming freeRows is implemented in row.c

    // Free all columns in the table
    freeColumns(table->lastColumn);  // Assuming freeColumns is implemented in column.c

    // Finally, free the Table object itself
    free(table);
}

// Function to free all Table objects in a database, starting from the last one
void freeTables(Table* lastTable) {
    while (lastTable) {
        Table* temp = lastTable;
        lastTable = lastTable->prev;  // Move to the previous Table object

        freeTable(temp);  // Free the current Table object
    }
}
