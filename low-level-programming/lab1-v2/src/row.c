#include "../include/row.h"

// Serialization function for a Row object
void serializeRow(FILE *file, const Row *row) {
    if (!file || !row) return;

    if (row->fileRow.offset) fseek(file, row->fileRow.offset, SEEK_SET);

    // Write the FileRow data (offsets)
    fwrite(&(row->fileRow), sizeof(row->fileRow), 1, file);

    // Additional serialization logic for the Row can be added here
}

// Deserialization function for a Row object
void deserializeRow(FILE *file, Row *row) {
    if (!file || !row) return;

    if (row->fileRow.offset) fseek(file, row->fileRow.offset, SEEK_SET);

    // Read the FileRow data (offsets)
    fread(&(row->fileRow), sizeof(row->fileRow), 1, file);

    // Additional deserialization logic for the Row can be added here
    // Note: Deserialization of the actual Data elements in the row is handled separately
}

// Function to get the next Row node
Row* getNextRow(FILE *file, Row *currentRow) {
    if (!currentRow) return NULL;
    if (currentRow->next) return currentRow->next;

    // Load from file if the next node is not in memory and file is provided
    if (file && currentRow->fileRow.nextRowOffset != 0) {
        Row* nextRow = malloc(sizeof(Row));
        if (!nextRow) return NULL;

        fseek(file, currentRow->fileRow.nextRowOffset, SEEK_SET);
        deserializeRow(file, nextRow);
        currentRow->next = nextRow;
        return nextRow;
    }

    return NULL;
}

// Function to get the previous Row node
Row* getPrevRow(FILE *file, Row *currentRow) {
    if (!currentRow) return NULL;
    if (currentRow->prev) return currentRow->prev;

    // Load from file if the previous node is not in memory and file is provided
    if (file && currentRow->fileRow.prevRowOffset != 0) {
        Row* prevRow = malloc(sizeof(Row));
        if (!prevRow) return NULL;

        fseek(file, currentRow->fileRow.prevRowOffset, SEEK_SET);
        deserializeRow(file, prevRow);
        currentRow->prev = prevRow;
        return prevRow;
    }

    return NULL;
}

// Function to get the last Data element in the Row
Data* getLastData(FILE *file, Row *currentRow) {
    if (!currentRow) return NULL;
    if (currentRow->lastData) return currentRow->lastData;

    // Load from file if the lastData is not in memory and file is provided
    if (file && currentRow->fileRow.lastDataOffset != 0) {
        Data* lastData = malloc(sizeof(Data));
        if (!lastData) return NULL;

        fseek(file, currentRow->fileRow.lastDataOffset, SEEK_SET);
        deserializeData(file, lastData);  // Assuming deserializeData is implemented elsewhere
        currentRow->lastData = lastData;
        return lastData;
    }

    return NULL;
}

#include "row.h"

// Function to free a single Row object
void freeRow(Row* row) {
    if (!row) return;

    // Free all Data objects in the row
    freeDatas(row->lastData);

    // Finally, free the Row object itself
    free(row);
}

// Function to free all Row objects in a table starting from the last one
void freeRows(Row* lastRow) {
    while (lastRow) {
        Row* temp = lastRow;
        lastRow = lastRow->prev;  // Move to the previous Row object

        freeRow(temp);  // Free the current Row object
    }
}

