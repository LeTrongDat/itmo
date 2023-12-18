#ifndef TABLE_H
#define TABLE_H

#include "row.h"
#include "column.h"
#include <stdio.h>
#include <stdlib.h>

// Structure to manage table-related file offsets
typedef struct {
    long offset;             // Offset of this Table in the file
    long nextTableOffset;    // Offset to the next Table in the file
    long prevTableOffset;    // Offset to the previous Table in the file
    long lastRowOffset;      // Offset to the last Row in the table (for file-based operations)
    long lastColumnOffset;   // Offset to the last Column in the table (for file-based operations)
} FileTable;

// Table structure
typedef struct Table {
    char tableName[32];      // Name of the Table
    FileTable fileTable;     // File-related metadata for the table
    Row *lastRow;            // Pointer to the last Row in the table
    Column *lastColumn;      // Pointer to the last Column in the table
    struct Table *prev;      // Pointer to the previous Table
    struct Table *next;      // Pointer to the next Table
    size_t numColumns;       // Number of columns in the table
    size_t numRows;          // Number of rows in the table
} Table;

// Serialization and deserialization function prototypes
void serializeTable(FILE *file, const Table *table);
void deserializeTable(FILE *file, Table *table);

// Helper function prototypes for navigating through tables
Table* getNextTable(FILE *file, Table *currentTable);
Table* getPrevTable(FILE *file, Table *currentTable);

// New helper functions to get the last column and last row
Column* getLastColumn(FILE *file, Table *table);
Row* getLastRow(FILE *file, Table *table);

// Function to free a single Table object
void freeTable(Table* table);

// Function to free all Table objects in a database
void freeTables(Table* lastTable);

#endif // TABLE_H
