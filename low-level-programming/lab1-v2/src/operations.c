#include "../include/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new Database and initialize its file
Database* createDatabase(const char* name) {
    if (!name) return NULL;

    // Allocate memory for the new Database
    Database* db = malloc(sizeof(Database));
    if (!db) return NULL;

    // Set the database name
    strncpy(db->databaseName, name, sizeof(db->databaseName) - 1);
    db->databaseName[sizeof(db->databaseName) - 1] = '\0';  // Ensure null termination

    // Initialize the FileDatabase structure
    db->fileDb.offset = 0;            // This will be updated when the database is saved
    db->fileDb.lastTableOffset = 0;   // Initially, there are no tables

    // Initialize other fields
    db->lastTable = NULL;
    db->numTables = 0;

    // Create and open a file for the database
    char filename[256];
    snprintf(filename, sizeof(filename), "%s", name);
    FILE* file = fopen(filename, "w+");
    if (!file) {
        // File creation failed, cleanup and return NULL
        free(db);
        return NULL;
    }

    // Serialize the initial state of the database to the file
    serializeDatabase(file, db);

    // Close the file
    fclose(file);

    return db;
}

// Function to load a Database from a file
Database* loadDatabase(const char* fileName) {
    if (!fileName) return NULL;

    // Open the file for reading
    FILE* file = fopen(fileName, "r+");
    if (!file) return NULL;

    // Allocate memory for the Database
    Database* db = malloc(sizeof(Database));
    if (!db) {
        fclose(file);
        return NULL;
    }

    // Deserialize the database from the file
    deserializeDatabase(file, db);

    // Close the file
    fclose(file);

    return db;
}

// Function to drop (delete) a Database and its contents
void dropDatabase(Database* db) {
    if (!db) return;

    // If the database is associated with a file and you wish to delete the file
    // when dropping the database, include file deletion logic here.
    char filename[256];
    snprintf(filename, sizeof(filename), "%s", db->databaseName);
    remove(filename);  // Deletes the file named db->databaseName.db

    // Free all resources associated with the database
    freeDatabase(db);  // Assuming freeDatabase is implemented in db.c
}

// Function to create a new Table in a Database
Table* createTable(Database* db, const char* tableName) {
    if (!db || !tableName) return NULL;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        return NULL;
    }

    // Move to the end of the file to get the offset for the new table
    fseek(file, 0, SEEK_END);
    long newTableOffset = ftell(file);

    // Allocate memory for the new Table
    Table* table = malloc(sizeof(Table));
    if (!table) {
        fclose(file);
        return NULL;
    }

    // Initialize the new Table
    strncpy(table->tableName, tableName, sizeof(table->tableName) - 1);
    table->tableName[sizeof(table->tableName) - 1] = '\0'; // Ensure null termination
    table->fileTable.offset = newTableOffset;
    table->fileTable.lastRowOffset = 0;
    table->fileTable.lastColumnOffset = 0;
    table->lastRow = NULL;
    table->lastColumn = NULL;
    table->numColumns = 0;
    table->numRows = 0;
    table->prev = NULL;  // Will be set after checking the last table
    table->next = NULL;

    // Check and update the last table in the database
    Table* lastTable = getLastTable(db);
    if (lastTable) {
        lastTable->fileTable.nextTableOffset = newTableOffset;
        lastTable->next = table;

        table->prev = lastTable;
        table->fileTable.prevTableOffset = lastTable->fileTable.offset;

        serializeTable(file, lastTable);  // Serialize the updated last table
    }

    // Update the database's last table reference and serialize the new table
    db->lastTable = table;
    db->fileDb.lastTableOffset = newTableOffset;
    db->numTables++;
    serializeTable(file, table);  // Serialize the new table
    serializeDatabase(file, db);

    // Close the file
    fclose(file);

    return table;
}

// Function to drop (delete) a Table from a Database by its name
void dropTable(Database* db, const char* tableName) {
    if (!db || !tableName) return;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        return;
    }

    // Find the table by name using the getTableByName function
    Table* currentTable = getTableByName(db, tableName);
    if (!currentTable) {
        fclose(file);
        return;
    }

    // Update the linked list of tables and their offsets in the file
    if (currentTable->prev) {
        currentTable->prev->next = currentTable->next;
        currentTable->prev->fileTable.nextTableOffset = (currentTable->next) ? currentTable->next->fileTable.offset : 0;
        serializeTable(file, currentTable->prev);
    }
    if (currentTable->next) {
        currentTable->next->prev = currentTable->prev;
        currentTable->next->fileTable.prevTableOffset = (currentTable->prev) ? currentTable->prev->fileTable.offset : 0;
        serializeTable(file, currentTable->next);
    }

    // If this is the last table, update the database's lastTable pointer
    if (db->lastTable == currentTable) {
        db->lastTable = currentTable->prev;
        db->fileDb.lastTableOffset = (db->lastTable) ? db->lastTable->fileTable.offset : 0;
    }

    // Update the table count in the database
    db->numTables--;

    serializeDatabase(file, db);

    // Free the resources associated with the table
    freeTable(currentTable);

    // Close the file
    fclose(file);
}


// Function to add a new Column to a Table in a Database
Column* addColumn(Database* db, const char* tableName, const char* columnName, DataType type) {
    if (!db || !tableName || !columnName) return NULL;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        return NULL;
    }

    // Find the table by name
    Table* table = getTableByName(db, tableName);
    if (!table) {
        fclose(file);
        return NULL;
    }

    // Allocate memory for the new Column
    Column* column = malloc(sizeof(Column));
    if (!column) {
        fclose(file);
        return NULL;
    }

    // Initialize the new Column
    strncpy(column->columnName, columnName, sizeof(column->columnName) - 1);
    column->columnName[sizeof(column->columnName) - 1] = '\0'; // Ensure null termination
    column->columnType = type;

    // Move to the end of the file to get the offset for the new column
    fseek(file, 0, SEEK_END);
    long newColumnOffset = ftell(file);

    // Set up the fileColumn data for the new column
    column->fileColumn.offset = newColumnOffset;
    column->fileColumn.nextColumnOffset = 0; // No next column yet
    column->fileColumn.prevColumnOffset = (table->lastColumn) ? table->lastColumn->fileColumn.offset : 0;

    // Update the column list in the table
    if (table->lastColumn) {
        table->lastColumn->next = column;
        table->lastColumn->fileColumn.nextColumnOffset = newColumnOffset;
        serializeColumn(file, table->lastColumn); // Update the last column in the file
    }
    column->prev = table->lastColumn;
    column->next = NULL;
    table->lastColumn = column;
    table->numColumns++;
    table->fileTable.lastColumnOffset = newColumnOffset;

    serializeColumn(file, column);
    serializeTable(file, table);

    // Close the file
    fclose(file);

    return column;
}


// Function to insert Data into a Row in a Table
void insertDataIntoRow(FILE* file, Row* row, Data* data) {
    if (!file || !row || !data) return;

    // Move to the end of the file to get the offset for the new data
    fseek(file, 0, SEEK_END);
    long newDataOffset = ftell(file);

    // Initialize the fileData for the new data
    data->fileData.offset = newDataOffset;
    data->fileData.nextDataOffset = 0; // No next data yet

    if (data->type == DT_STRING) {
        // Calculate the length of the string including the null terminator
        data->dataSize = strlen(data->value.string_val) + 1;
    }

    Data *lastData = row->lastData; //getLastData(file, row);
    data->fileData.prevDataOffset = (lastData) ? lastData->fileData.offset : 0;

    // Add the data to the row's data list
    if (row->lastData) {
        row->lastData->next = data;
        row->lastData->fileData.nextDataOffset = newDataOffset;
        // Update the previous data element in the file
        fseek(file, row->lastData->fileData.offset, SEEK_SET);
        serializeData(file, row->lastData);
    }
    data->prev = row->lastData;
    data->next = NULL;
    row->lastData = data;

    // Serialize the new data to the file
    serializeData(file, data);

    // Update lastDataOffset for the row
    row->fileRow.lastDataOffset = newDataOffset;

    serializeRow(file, row);
}

// Function to insert a new Row into a Table in a Database
void insertRow(Database* db, const char* tableName, Data* data) {
    if (!db || !tableName) return;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        return;
    }

    // Find the table by name
    Table* table = getTableByName(db, tableName);
    if (!table) {
        fclose(file);
        return;
    }

    // Allocate memory for the new Column
    Row* row = malloc(sizeof(Row));
    if (!row) {
        fclose(file);
        return;
    }

    // Move to the end of the file to get the offset for the new row
    fseek(file, 0, SEEK_END);
    long newRowOffset = ftell(file);

    // Initialize the fileRow data for the new row
    row->fileRow.offset = newRowOffset;
    row->fileRow.nextRowOffset = 0; // No next row yet
    row->fileRow.prevRowOffset = (table->lastRow) ? table->lastRow->fileRow.offset : 0;
    row->fileRow.lastDataOffset = 0; // To be updated when data is inserted

    // Update the row list in the table
    table->lastRow = getLastRow(file, table);
    if (table->lastRow) {
        table->lastRow->next = row;
        table->lastRow->fileRow.nextRowOffset = newRowOffset;
        fseek(file, table->lastRow->fileRow.offset, SEEK_SET);
        serializeRow(file, table->lastRow); // Update the last row in the file
    }
    row->prev = table->lastRow;
    row->next = NULL;
    table->lastRow = row;
    table->numRows++;

    serializeRow(file, row);
    

    for(int i = 0; i < table->numColumns; i++) {
        insertDataIntoRow(file, row, &data[i]);
    }

    // Update the table's lastRowOffset and serialize the updated table
    table->fileTable.lastRowOffset = newRowOffset;
    serializeTable(file, table);

    // // Close the file
    fclose(file);
}


// Function to select rows from a Table in a Database based on a condition
RowNode* selectRows(Database* db, const char* tableName, ConditionFunc condition) {

    if (!db || !tableName || !condition) return NULL;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        return NULL;
    }

    // Find the table by name

    printf("pre getlastrow 1 %d\n", db->lastTable->lastRow->lastData->fileData.offset);
    Table* table = getTableByName(db, tableName);
    printf("pre getlastrow 3 %d\n", table->lastRow->lastData->fileData.offset);
    if (!table) {
        fclose(file);
        return NULL;
    }

    // Iterate over rows in the table and apply the condition function
    RowNode* head = NULL;
    RowNode* tail = NULL;
    Row* currentRow = malloc(sizeof(Row));

    currentRow = getLastRow(file, table);
    while (currentRow) {
        printf("curren row %d\n", currentRow->lastData->fileData.offset);
        if (condition(file, currentRow)) {
            // Create a new RowNode for the matching row
            RowNode* newNode = malloc(sizeof(RowNode));
            if (!newNode) {
                // Handle memory allocation failure
                // ...
                break;
            }
            newNode->row = currentRow;
            newNode->next = NULL;

            // Add the new node to the list of matching rows
            if (!head) {
                head = newNode;
            } else {
                tail->next = newNode;
            }
            tail = newNode;
        }
        currentRow = getPrevRow(file, currentRow); // Assuming getPrevRow function exists
    }

    // Close the file
    fclose(file);

    return head; // Head of the list of matching rows
}

// Function to update rows in a Table based on a condition
void updateRow(Database* db, const char* tableName, Data* newData, ConditionFunc condition) {
    if (!db || !tableName || !newData || !condition) return;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        // Handle file opening failure
        // ...
        return;
    }

    // Find the table by name
    Table* table = getTableByName(db, tableName);
    if (!table) {
        // Table not found, handle the error
        // ...
        fclose(file);
        return;
    }

    // Iterate over rows in the table and apply the condition function
    Row* currentRow = table->lastRow;
    while (currentRow) {
        if (condition(db, currentRow)) {
            // Update the data in the row
            Data* currentData = currentRow->lastData;
            Data* newDataPtr = newData;
            while (currentData && newDataPtr) {
                // Check for string data type
                if (currentData->type == DT_STRING && newDataPtr->type == DT_STRING) {
                    size_t newStringLength = strlen(newDataPtr->value.string_val);
                    if (newStringLength > currentData->dataSize) {
                        // New string size exceeds allocated space, handle the error
                        // ...
                        break;
                    }
                    strncpy(currentData->value.string_val, newDataPtr->value.string_val, newStringLength);
                    currentData->dataSize = newStringLength;
                } else {
                    // For other data types, direct assignment
                    currentData->value = newDataPtr->value;
                }

                // Serialize the updated data to the file
                serializeData(file, currentData);

                currentData = currentData->prev; // Move to the previous data element
                newDataPtr = newDataPtr->next; // Move to the next new data element
            }
        }
        currentRow = getPrevRow(file, currentRow); // Assuming getPrevRow function exists
    }

    // Close the file
    fclose(file);
}

// Function to delete rows from a Table in a Database based on a condition
void deleteRow(Database* db, const char* tableName, ConditionFunc condition) {
    if (!db || !tableName || !condition) return;

    // Open the database file
    FILE* file = fopen(db->databaseName, "r+");
    if (!file) {
        // Handle file opening failure
        // ...
        return;
    }

    // Find the table by name
    Table* table = getTableByName(db, tableName);
    if (!table) {
        // Table not found, handle the error
        // ...
        fclose(file);
        return;
    }

    Row* currentRow = table->lastRow;
    Row* prevRow = NULL;

    while (currentRow) {
        if (condition(db, currentRow)) {
            // Update the fileRow offsets for adjacent rows
            if (prevRow) {
                prevRow->fileRow.nextRowOffset = currentRow->fileRow.nextRowOffset;
                fseek(file, prevRow->fileRow.offset, SEEK_SET);
                serializeRow(file, prevRow);
            }
            if (currentRow->next) {
                currentRow->next->fileRow.prevRowOffset = (prevRow) ? prevRow->fileRow.offset : 0;
                fseek(file, currentRow->next->fileRow.offset, SEEK_SET);
                serializeRow(file, currentRow->next);
            }

            // Update the table's lastRow if needed
            if (table->lastRow == currentRow) {
                table->lastRow = prevRow;
                table->fileTable.lastRowOffset = (prevRow) ? prevRow->fileRow.offset : 0;
            }

            // Delete the row and free its resources
            Row* rowToDelete = currentRow;
            currentRow = getPrevRow(file, currentRow);  // Move to the previous row
            freeRow(rowToDelete);

            table->numRows--;

        } else {
            prevRow = currentRow;
            currentRow = getPrevRow(file, currentRow);  // Move to the previous row
        }
    }

    // Serialize the updated table to the file
    fseek(file, table->fileTable.offset, SEEK_SET);
    serializeTable(file, table);

    // Close the file
    fclose(file);
}

