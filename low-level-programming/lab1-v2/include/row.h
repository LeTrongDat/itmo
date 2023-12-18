#ifndef ROW_H
#define ROW_H

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

// Structure to manage row-related file offsets
typedef struct {
    long offset;          // Offset of this Row in the file
    long nextRowOffset;   // Offset to the next Row in the file
    long prevRowOffset;   // Offset to the previous Row in the file
    long lastDataOffset;  // Offset to the last Data in this Row (for file-based operations)
} FileRow;

// Row structure
typedef struct Row {
    FileRow fileRow;      // File-related metadata for the row
    Data *lastData;       // Pointer to the last Data element in this Row
    struct Row *prev;     // Pointer to the previous Row
    struct Row *next;     // Pointer to the next Row
} Row;

// Serialization and deserialization function prototypes
void serializeRow(FILE *file, const Row *row);
void deserializeRow(FILE *file, Row *row);

// Helper function prototypes for navigating through rows and accessing data
Row* getNextRow(FILE *file, Row *currentRow);
Row* getPrevRow(FILE *file, Row *currentRow);
Data* getLastData(FILE *file, Row *currentRow);

// Function to free a single Row object
void freeRow(Row* row);

// Function to free all Row objects in a table
void freeRows(Row* lastRow);

#endif // ROW_H
