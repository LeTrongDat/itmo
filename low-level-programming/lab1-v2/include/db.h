#ifndef DB_H
#define DB_H

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For string comparison

// Structure to manage database-related file offsets
typedef struct {
    long offset;             // Offset of this Database in the file
    long lastTableOffset;    // Offset to the last Table in the database (for file-based operations)
} FileDatabase;

// Database structure
typedef struct Database {
    char databaseName[32];   // Name of the Database
    FileDatabase fileDb;     // File-related metadata for the database
    Table *lastTable;        // Pointer to the last Table in the database
    size_t numTables;        // Number of tables in the database
} Database;

// Serialization and deserialization function prototypes
void serializeDatabase(FILE *file, const Database *db);
void deserializeDatabase(FILE *file, Database *db);

// Helper function prototypes for navigating through databases
Table* getLastTable(Database *db);
Table* getTableByName(Database *db, const char *tableName);

// Function to free a Database object
void freeDatabase(Database* db);

#endif // DB_H
