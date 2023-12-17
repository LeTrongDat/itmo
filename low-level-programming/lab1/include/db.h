#ifndef DB_H
#define DB_H

#include "table.h"
#include "data_types.h" // Include the data types definitions

// Structure for the database
typedef struct {
    char *filename;      // Filename for storing the database
    Table **tables;      // Array of tables in the database
    int tableCount;      // Number of tables
} Database;

// Function to initialize a database from a file
Database *initDatabase(const char *filename);

// New function to create a new database
Database *createDatabase(const char *filename);  

// Function to create a new table in the database
Table *createTable(Database *db, const char *tableName, TableColumn *columns, int numColumns);

// Function to drop (delete) a table from the database
void dropTable(Database *db, const char *tableName);

// Function to free the memory allocated for the database
void freeDatabase(Database *db);

// Define a type for a comparison function used in querying
typedef bool (*ComparisonFunc)(Data *data);

// Function to insert data into a table
// Returns the number of rows affected, -1 on failure
int insertData(Database *db, const char *tableName, Data *data, int numFields);

// Function to update data in a table based on a comparison function
// Returns the number of rows affected, -1 on failure
int updateData(Database *db, const char *tableName, ComparisonFunc compFunc, Data *newData, int numFields);

// Function to delete data from a table based on a comparison function
// Returns the number of rows affected, -1 on failure
int deleteData(Database *db, const char *tableName, ComparisonFunc compFunc);

// Function to query data from a table based on a comparison function
// Returns the number of rows affected (or found), -1 on failure
int queryData(Database *db, const char *tableName, ComparisonFunc compFunc, Data **outDataArray);

#endif // DB_H
