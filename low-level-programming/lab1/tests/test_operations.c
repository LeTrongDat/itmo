#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/operations.h"
#include "../include/utilities.h"

void testCreateDatabase() {
    printf("Testing createDatabase...\n");

    Database* db = createDatabase("TestDB");
    if (db == NULL) {
        printf("Test Failed: createDatabase returned NULL.\n");
    } else {
        printf("Test Passed: Database object created.\n");

        // Check if the database file is created
        FILE* file = fopen("TestDB.db", "r");
        if (file) {
            printf("Test Passed: Database file 'TestDB.db' exists.\n");
            fclose(file);
        } else {
            printf("Test Failed: Database file 'TestDB.db' does not exist.\n");
        }

        // Clean up: Close and delete the database
        closeDatabase(db);
        remove("TestDB.db");
    }
}

void testOpenDatabase() {
    printf("Testing openDatabase...\n");

    // Create and immediately close a database to ensure the file exists
    Database* dbCreated = createDatabase("TestDB");
    if (dbCreated == NULL) {
        printf("Preparation Failed: Unable to create 'TestDB' for testing.\n");
        return;
    }
    closeDatabase(dbCreated);

    // Attempt to open the existing database
    Database* dbOpened = openDatabase("TestDB");
    if (dbOpened == NULL) {
        printf("Test Failed: openDatabase returned NULL.\n");
    } else {
        printf("Test Passed: Database 'TestDB' opened successfully.\n");

        closeDatabase(dbOpened);
    }

    // Clean up
    remove("TestDB.db");
}

void testCloseDatabase() {
    printf("Testing closeDatabase...\n");

    // Create a new database
    Database* db = createDatabase("TestDB");
    if (db == NULL) {
        printf("Preparation Failed: Unable to create 'TestDB' for testing.\n");
        return;
    }

    // Perform some operations (like creating a table)
    createTable(db, "TestTable");

    // Close the database
    closeDatabase(db);

    createTable(db, "AnotherTable");
    
    if (db == NULL || db->dbConnection == NULL) 
        printf("Database 'TestDB' closed. Further operations should be invalid.\n");

    // Clean up
    remove("TestDB.db");
}

void testDropDatabase() {
    printf("Testing dropDatabase...\n");

    // Create and close a database to ensure the file exists
    Database* db = createDatabase("TestDB");
    if (db == NULL) {
        printf("Preparation Failed: Unable to create 'TestDB' for testing.\n");
        return;
    }
    closeDatabase(db);

    // Check if the database file exists before dropping
    FILE* file = fopen("TestDB.db", "r");
    if (file) {
        fclose(file);
        printf("Database file 'TestDB.db' exists, proceeding to drop.\n");
    } else {
        printf("Preparation Failed: Database file 'TestDB.db' does not exist.\n");
        return;
    }

    // Drop the database
    dropDatabase("TestDB");

    // Verify that the file no longer exists
    file = fopen("TestDB.db", "r");
    if (!file) {
        printf("Test Passed: Database file 'TestDB.db' successfully deleted.\n");
    } else {
        fclose(file);
        printf("Test Failed: Database file 'TestDB.db' still exists.\n");
    }
}

bool isTableExists(Database* db, const char* tableName) {
    if (db == NULL || tableName == NULL) {
        return false;
    }

    Table* table = getTableByName(db, tableName);
    if (table != NULL) {
        return true;
    }

    return false;
}

void testCreateTable() {
    printf("Testing createTable...\n");

    // Create a new database
    Database* db = createDatabase("TestDB");
    if (db == NULL) {
        printf("Preparation Failed: Unable to create 'TestDB' for testing.\n");
        return;
    }

    // Create a new table
    createTable(db, "TestTable");

    // Verify that the table was created
    if (isTableExists(db, "TestTable")) {
        printf("Test Passed: Table 'TestTable' successfully created.\n");
    } else {
        printf("Test Failed: Table 'TestTable' not found in the database.\n");
    }

    // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

void testAddColumn() {
    printf("Testing addColumn...\n");

    // Create a new database and table
    Database* db = createDatabase("TestDB");
    createTable(db, "TestTable");

    // Define a new column
    ColumnMetadata newColumn = {{0, "TestColumn"}, INTEGER};

    // Add the new column to the table
    addColumn(db, "TestTable", newColumn);

    // Verify that the column was added
    if (isColumnExists(db, "TestTable", "TestColumn")) {
        printf("Test Passed: Column 'TestColumn' successfully added to 'TestTable'.\n");
    } else {
        printf("Test Failed: Column 'TestColumn' not found in 'TestTable'.\n");
    }

    // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

void testAddRow() {
    printf("Testing addRow...\n");

    // Create a new database and table
    Database* db = createDatabase("TestDB");
    createTable(db, "TestTable");

    // Add columns to the table
    ColumnMetadata idColumn = {{2, "ID"}, INTEGER};
    ColumnMetadata nameColumn = {{4, "Name"}, STRING};

    addColumn(db, "TestTable", idColumn);
    addColumn(db, "TestTable", nameColumn);

    Data rowData[2];
    rowData[0].type = INTEGER;
    rowData[0].value.intValue = 1;
    rowData[1].type = STRING;
    rowData[1].value.strValue = "John Doe"; 

    // // Add a new row
    addRow(db, "TestTable", rowData);

    Row* firstRow = getFirstRow(db, "TestTable");
    if (firstRow != NULL) {
        printf("First row retrieved successfully.\n");

        Data* dataID = getDataByIndex(db, "TestTable", firstRow, 0);
        Data* dataName = getDataByIndex(db, "TestTable", firstRow, 1); 

        if (dataID != NULL && dataID->type == INTEGER && dataID->value.intValue == 1) {
            printf("ID data verification passed.\n");
        } else {
            printf("ID data verification failed.\n");
        }

        if (dataName != NULL && dataName->type == STRING && strcmp(dataName->value.strValue, "John Doe") == 0) {
            printf("Name data verification passed.\n");
        } else {
            printf("Name data verification failed.\n");
        }

    } else {
        printf("No rows found in the table.\n");
    }
    // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

void setupDatabaseForTesting(Database** db, const char* dbName, const char* tableName) {
    // Create a new database
    *db = createDatabase(dbName);
    if (*db == NULL) {
        fprintf(stderr, "Failed to create database '%s'.\n", dbName);
        exit(1); // Exit or handle the error as appropriate
    }

    createTable(*db, tableName);

    ColumnMetadata idColumn = {{2, "ID"}, INTEGER};
    ColumnMetadata nameColumn = {{4, "Name"}, STRING};

    addColumn(*db, tableName, idColumn);
    addColumn(*db, tableName, nameColumn);

    for (int i = 1; i <= 100; ++i) {
        Data rowData[2];
        char name[50];
        sprintf(name, "Person %d", i);

        // rowData[0].type = INTEGER;
        rowData[0].value.intValue = i;
        // rowData[1].type = STRING;
        rowData[1].value.strValue = name;

        addRow(*db, tableName, rowData);
    }
}

bool examplePredicate(Database* db, Row* row, PredicateContext* context) {
    Data* data = getDataByIndex(db, "TestTable", row, 0); 
    bool result = (data != NULL && data->type == INTEGER && data->value.intValue > 69);
    return result;
}

void testQueryRows() {
    printf("Testing queryRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "TestDB", "TestTable");

    // Query rows using the predicate
    RowNode* queriedRows = queryRows(db, "TestTable", examplePredicate, NULL);

    // // Verify the query result
    int matchedRowCount = 0;
    for (RowNode* node = queriedRows; node != NULL; node = node->next) {
        matchedRowCount++;
    }

    printf("Number of rows matching the predicate: %d\n", matchedRowCount);

    if (matchedRowCount == 31) {
        printf("Test Passed: 31 matching rows found\n");
    }

    // Free resources allocated for queried rows
    while (queriedRows != NULL) {
        RowNode* temp = queriedRows;
        queriedRows = queriedRows->next;
        free(temp);
    }

    // // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

bool updatePredicate(Database* db, Row* row, PredicateContext* context) {
    Data* data = getDataByIndex(db, "TestTable", row, 0); // Check first column (ID)
    bool result = (data != NULL && data->type == INTEGER && data->value.intValue <= 50);
    return result;
}

void testUpdateRows() {
    printf("Testing updateRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "TestDB", "TestTable");

    // Prepare new data for update
    Data newData[2] = {{INTEGER, .value.intValue = 999}, {STRING, .value.strValue = "Updated Name"}};

    // Update rows based on the predicate
    updateRows(db, "TestTable", updatePredicate, newData, NULL);

    // Verify the update
    Row* row = getFirstRow(db, "TestTable");
    int updatedCount = 0;
    int rowCount = 0;
    while (row != NULL) {
        rowCount++;
        Data* updatedID = getDataByIndex(db, "TestTable", row, 0);
        Data* updatedName = getDataByIndex(db, "TestTable", row, 1);

        if (updatedID->value.intValue == 999 && strcmp(updatedName->value.strValue, "Updated Name") == 0) {
            updatedCount++;
        }
        row = getNextRow(db, row);
    }
    printf("Number of rows: %d\n", rowCount);
    printf("Number of rows updated: %d\n", updatedCount);

    if (updatedCount == 50) {
        printf("Test Passed: 50 matching rows found\n");
    }

    // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

bool deletePredicate(Database* db, Row* row, PredicateContext *context) {
    Data* data = getDataByIndex(db, "TestTable", row, 0); // Check first column (ID)
    bool result = (data != NULL && data->type == INTEGER && data->value.intValue <= 22);
    // Optionally free data if necessary
    return result;
}

void testDeleteRows() {
    printf("Testing deleteRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "TestDB", "TestTable");

    // Delete rows based on the predicate
    deleteRows(db, "TestTable", deletePredicate, NULL);

    // Verify the delete operation
    Row* row = getFirstRow(db, "TestTable");
    int remainingCount = 0;
    while (row != NULL) {
        if (!deletePredicate(db, row, NULL)) {
            remainingCount++;
        }
        row = getNextRow(db, row);
    }

    printf("Number of rows remaining after delete: %d\n", remainingCount);

    // Clean up
    closeDatabase(db);
    remove("TestDB.db");
}

int main() {
    testCreateDatabase();
    testOpenDatabase();
    testCloseDatabase();
    testDropDatabase();
    testCreateTable();
    testAddColumn();
    testAddRow();
    testQueryRows();
    testUpdateRows();
    testDeleteRows();
    return 0;
}
