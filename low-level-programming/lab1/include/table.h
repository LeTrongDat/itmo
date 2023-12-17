#ifndef TABLE_H
#define TABLE_H

#include "data_types.h"  // Include the definitions of data types

// Structure defining a column in a table
typedef struct {
    char *name;        // Name of the column
    DataType dataType; // Data type of the column (int, float, string, bool, etc.)
} TableColumn;

// Structure defining a row in a table
// This is implemented as a linked list node for easy row manipulation
typedef struct TableRow {
    Data *data;             // Array of data, each element corresponds to a column
    struct TableRow *next;  // Pointer to the next row in a linked list
} TableRow;

// Structure defining a table
typedef struct {
    char *name;            // Name of the table
    TableColumn *columns;  // Array of columns in the table
    int numColumns;        // Number of columns in the table
    TableRow *rows;        // Pointer to the first row in the table
} Table;

#endif // TABLE_H
