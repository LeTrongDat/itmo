#include "../include/column.h"

// Serialization function for a Column object
void serializeColumn(FILE *file, const Column *column) {
    if (!file || !column) return;

    if (column->fileColumn.offset) fseek(file, column->fileColumn.offset, SEEK_SET);

    // Write the column name
    fwrite(column->columnName, sizeof(column->columnName), 1, file);

    // Write the column type
    fwrite(&(column->columnType), sizeof(column->columnType), 1, file);

    // Write the FileColumn data (offsets)
    fwrite(&(column->fileColumn), sizeof(column->fileColumn), 1, file);
}

// Deserialization function for a Column object
void deserializeColumn(FILE *file, Column *column) {
    if (!file || !column) return;

    if (column->fileColumn.offset) fseek(file, column->fileColumn.offset, SEEK_SET);

    // Read the column name
    fread(column->columnName, sizeof(column->columnName), 1, file);

    // Read the column type
    fread(&(column->columnType), sizeof(column->columnType), 1, file);

    // Read the FileColumn data (offsets)
    fread(&(column->fileColumn), sizeof(column->fileColumn), 1, file);
}

// Function to get the next Column node
Column* getNextColumn(FILE *file, Column *currentColumn) {
    if (!currentColumn) return NULL;
    if (currentColumn->next) return currentColumn->next;

    // Load from file if the next node is not in memory and file is provided
    if (file && currentColumn->fileColumn.nextColumnOffset != 0) {
        Column* nextColumn = malloc(sizeof(Column));
        if (!nextColumn) return NULL;

        fseek(file, currentColumn->fileColumn.nextColumnOffset, SEEK_SET);
        deserializeColumn(file, nextColumn);
        currentColumn->next = nextColumn;
        return nextColumn;
    }

    return NULL;
}

// Function to get the previous Column node
Column* getPrevColumn(FILE *file, Column *currentColumn) {
    if (!currentColumn) return NULL;
    if (currentColumn->prev) return currentColumn->prev;

    // Load from file if the previous node is not in memory and file is provided
    if (file && currentColumn->fileColumn.prevColumnOffset != 0) {
        Column* prevColumn = malloc(sizeof(Column));
        if (!prevColumn) return NULL;

        fseek(file, currentColumn->fileColumn.prevColumnOffset, SEEK_SET);
        deserializeColumn(file, prevColumn);
        currentColumn->prev = prevColumn;
        return prevColumn;
    }

    return NULL;
}

// Function to free a single Column object
void freeColumn(Column* column) {
    if (!column) return;

    // Finally, free the Column object itself
    free(column);
}

// Function to free all Column objects in a table, starting from the last one
void freeColumns(Column* lastColumn) {
    while (lastColumn) {
        Column* temp = lastColumn;
        lastColumn = lastColumn->prev;  // Move to the previous Column object

        freeColumn(temp);  // Free the current Column object
    }
}
