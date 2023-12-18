#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "db.h"

// Linked list node for Rows
typedef struct RowNode {
    Row* row;
    struct RowNode* next;
} RowNode;

// Function pointer type for row condition evaluation
typedef bool (*ConditionFunc)(const Row*);

// Database management
Database* createDatabase(const char* name);
Database* loadDatabase(const char* fileName);
void dropDatabase(Database* db);

// Table management
Table* createTable(Database* db, const char* tableName);
void dropTable(Database* db, const char* tableName);

// Column management
Column* addColumn(Database* db, const char* tableName, const char* columnName, DataType type);

// Data manipulation
void insertRow(Database* db, const char* tableName, Data *data);  // Insert a new row
RowNode* selectRows(Database* db, const char* tableName, ConditionFunc condition);  // Select rows based on condition
void updateRow(Database* db, const char* tableName, Data *data, ConditionFunc condition);  // Update a row based on condition
void deleteRow(Database* db, const char* tableName, ConditionFunc condition);  // Delete a row based on condition

#endif // OPERATIONS_H
