#ifndef COLUMN_H
#define COLUMN_H

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

// Structure to manage column-related file offsets
typedef struct {
    long offset;            // Offset of this Column in the file
    long nextColumnOffset;  // Offset to the next Column in the file
    long prevColumnOffset;  // Offset to the previous Column in the file
} FileColumn;

// Column structure
typedef struct Column {
    char columnName[32];     // Name of the Column
    DataType columnType;     // Type of the Column
    FileColumn fileColumn;   // File-related metadata for the column
    struct Column *prev;     // Pointer to the previous Column
    struct Column *next;     // Pointer to the next Column
} Column;

// Serialization and deserialization function prototypes
void serializeColumn(FILE *file, const Column *column);
void deserializeColumn(FILE *file, Column *column);

// Helper function prototypes for navigating through columns
Column* getNextColumn(FILE *file, Column *currentColumn);
Column* getPrevColumn(FILE *file, Column *currentColumn);

// Function to free a single Column object
void freeColumn(Column* column);

// Function to free all Column objects in a table
void freeColumns(Column* lastColumn);

#endif // COLUMN_H
