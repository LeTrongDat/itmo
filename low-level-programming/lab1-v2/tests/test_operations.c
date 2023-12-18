#include "../include/operations.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_createDatabase() {
    printf("Running test for createDatabase...\n");

    // Setup
    const char* dbName = "test_db";

    // Test: Invoke createDatabase function
    Database* db = createDatabase(dbName);

    // Assertions
    assert(db != NULL);  // Assert that the database object is created
    assert(strcmp(db->databaseName, dbName) == 0);  // Assert database name matches

    // Cleanup using dropDatabase
    dropDatabase(db);

    // Check if the database file is removed
    FILE* file = fopen(dbName, "r+");
    assert(file == NULL);  // Assert that the file no longer exists

    printf("Test for createDatabase passed.\n");
}

void test_loadDatabase() {
    printf("Running test for loadDatabase...\n");

    // Setup: Create a database to load
    const char* dbName = "test_db";
    createDatabase(dbName);

    // Test: Invoke loadDatabase function
    Database* db = loadDatabase(dbName);

    // Assertions
    assert(db != NULL);  // Assert that the database object is loaded
    assert(strcmp(db->databaseName, dbName) == 0);  // Assert database name matches

    // Cleanup
    dropDatabase(db);

    printf("Test for loadDatabase passed.\n");
}

void test_dropDatabase() {
    printf("Running test for dropDatabase...\n");

    // Setup: Create a database to drop
    const char* dbName = "test_db";
    Database* db = createDatabase(dbName);

    // Test: Invoke dropDatabase function
    dropDatabase(db);

    // Assertions
    FILE* file = fopen(dbName, "r+");
    assert(file == NULL);  // Assert that the database file is removed

    printf("Test for dropDatabase passed.\n");
}

void test_createTable() {
    printf("Running tests for createTable...\n");

    // Create a database for testing
    const char* dbName = "test_db";
    Database* db = createDatabase(dbName);
    assert(db != NULL);

    // Test: Create multiple tables in the database
    const char* tableName1 = "table1";
    const char* tableName2 = "table2";
    Table* table1 = createTable(db, tableName1);
    Table* table2 = createTable(db, tableName2);

    assert(table1 != NULL && strcmp(table1->tableName, tableName1) == 0);
    assert(table2 != NULL && strcmp(table2->tableName, tableName2) == 0);
    assert(db->numTables == 2);

    // Cleanup
    dropDatabase(db);

    printf("Tests for createTable passed.\n");
}

void test_loadDatabase2() {
    printf("Running test for loadDatabase 2...\n");

    // Setup: Create a database and add tables
    const char* dbName = "test_db";
    Database* db = createDatabase(dbName);
    createTable(db, "table1");
    createTable(db, "table2");

    // Test: Load the database and check the tables
    db = loadDatabase(dbName);
    assert(db != NULL);
    assert(db->numTables == 2);

    // Check if the loaded tables are correct (additional checks might be needed based on implementation)

    // Cleanup
    dropDatabase(db);

    printf("Test for loadDatabase 2 passed.\n");
}

void test_dropTable() {
    printf("Running tests for dropTable...\n");

    // Create a database and tables for testing
    const char* dbName = "test_db";
    Database* db = createDatabase(dbName);
    assert(db != NULL);

    const char* tableName1 = "table1";
    const char* tableName2 = "table2";
    createTable(db, tableName1);
    createTable(db, tableName2);

    // Test 1: Drop the first table
    dropTable(db, tableName1);
    assert(db->numTables == 1);

    // Test 2: Drop the second table
    dropTable(db, tableName2);
    assert(db->numTables == 0);

    // Test 3: Try dropping a table that doesn't exist
    dropTable(db, "nonexistent_table");
    assert(db->numTables == 0); // Number of tables should remain unchanged

    // Cleanup
    dropDatabase(db);

    printf("Tests for dropTable passed.\n");
}

void test_addColumn() {
    printf("Running test for addColumn...\n");

    // Setup: Create a database and a table
    const char* dbName = "test_db";
    Database* db = createDatabase(dbName);
    const char* tableName = "test_table";
    Table* table = createTable(db, tableName);

    // Test: Add a column to the table
    const char* columnName = "test_column";
    DataType columnType = DT_INT;  // Assuming you have defined data types
    Column* column = addColumn(db, tableName, columnName, columnType);

    // Assertions
    assert(column != NULL);  // Assert that the column is created
    assert(strcmp(column->columnName, columnName) == 0);  // Assert column name matches
    assert(column->columnType == columnType);  // Assert column type matches
    assert(table->numColumns == 1);  // Assert that the table's column count is updated

    // Cleanup
    dropDatabase(db);

    printf("Test for addColumn passed.\n");
}

void createSampleTableWithDifferentTypes(Database* db, const char* tableName) {
    // Create table with columns of different data types
    createTable(db, tableName);
    addColumn(db, tableName, "col_int", DT_INT);    // Integer column
    addColumn(db, tableName, "col_str", DT_STRING); // String column
    addColumn(db, tableName, "col_float", DT_FLOAT); // Float column
    addColumn(db, tableName, "col_bool", DT_BOOL); // Bool column
}

void insertSampleRowsWithDifferentTypes(Database* db, const char* tableName) {
    // Insert rows with sample data of different types
    for (int i = 0; i < 10000; ++i) {
        Data rowData[4];

        // Integer data
        rowData[0].type = DT_INT;
        rowData[0].value.int_val = i;  // Sample integer value

        // String data
        rowData[1].type = DT_STRING;
        char* strBuffer = malloc(20 * sizeof(char)); // Allocate memory for the string
        if (strBuffer == NULL) {
            break;
        }
        sprintf(strBuffer, "string %d", i);  // Sample string value
        rowData[1].value.string_val = strBuffer;

        // Float data
        rowData[2].type = DT_FLOAT;
        rowData[2].value.float_val = (float)i * 1.1f;  // Sample float value

        // Custom data (assuming some custom type)
        rowData[3].type = DT_BOOL;
        rowData[3].value.bool_val = true;

        insertRow(db, tableName, rowData);
    }
}

void test_insertRowAndLoadDatabase() {
    printf("Running test for insertRow and loadDatabase...\n");

    // Setup: Create a database and a table with different data types
    const char* dbName = "test_db";
    const char* tableName = "test_table";
    Database* db = createDatabase(dbName);
    createSampleTableWithDifferentTypes(db, tableName);
    insertSampleRowsWithDifferentTypes(db, tableName);

    // // Load the database from the file
    Database* loadedDb = loadDatabase(dbName);
    assert(loadedDb != NULL);
    assert(loadedDb->numTables == 1);

    // // Retrieve the table and check its data
    Table* loadedTable = getTableByName(loadedDb, tableName);
    assert(loadedTable != NULL);
    assert(loadedTable->numColumns == 4);
    assert(loadedTable->numRows == 10000); // Check row count

    // Additional checks to verify the data of each row
    // (Depending on your implementation, you'll need to retrieve and verify each row's data)

    // Clean
    dropDatabase(loadedDb);

    printf("Test for insertRow and loadDatabase passed.\n");
}

int main() {
    test_createDatabase();
    test_loadDatabase();
    test_loadDatabase2();
    test_dropDatabase();
    test_createTable();
    test_dropTable();
    test_addColumn();
    test_insertRowAndLoadDatabase();
    // Add more tests as needed
    return 0;
}
