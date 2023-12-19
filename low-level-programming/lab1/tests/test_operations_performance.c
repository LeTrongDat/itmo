#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/operations.h"
#include "../include/utilities.h"


void setupDatabaseForTesting(Database** db, const char* dbName, const char* tableName) {
    // Create a new database
    *db = createDatabase(dbName);
    if (*db == NULL) {
        fprintf(stderr, "Failed to create database '%s'.\n", dbName);
        exit(1); // Exit or handle the error as appropriate
    }

    // Create a new table
    createTable(*db, tableName);

    // Add columns to the table
    ColumnMetadata idColumn = {{2, "ID"}, INTEGER};
    ColumnMetadata nameColumn = {{4, "Name"}, STRING};

    addColumn(*db, tableName, idColumn);
    addColumn(*db, tableName, nameColumn);

    // Insert multiple rows into the table
    for (int i = 1; i <= 100; ++i) {
        Data rowData[2];
        char name[50];
        sprintf(name, "Person %d", i); // Generating a name for each row

        // rowData[0].type = INTEGER;
        rowData[0].value.intValue = i;
        // rowData[1].type = STRING;
        rowData[1].value.strValue = name;

        addRow(*db, tableName, rowData);
    }
}

void testAddRowPerformance() {
    printf("Testing performance of addRow...\n");

    Database* db;
    setupDatabaseForTesting(&db, "PerfTestDB", "PerfTestTable");

    const int numberOfRowsToAdd = 10000; // Example large number of rows
    clock_t start, end;
    double cpu_time_used;

    FILE* file = fopen("addRowPerformance.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Write header to the CSV file
    fprintf(file, "RowNumber,TimeInSeconds\n");

    // Add a large number of rows
    for (int i = 0; i < numberOfRowsToAdd; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "Sample Name"}};
        start = clock();
        addRow(db, "PerfTestTable", rowData);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        fprintf(file, "%d,%f\n", i + 1, cpu_time_used);
    }

    // Clean up
    closeDatabase(db);
    remove("PerfTestDB.db");
}

bool queryAllPredicate(Database* db, const Row* row) {
    // Predicate that matches all rows
    return true;
}

void testQueryRowsPerformance() {
    printf("Testing performance of queryRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "PerfTestDB", "PerfTestTable");

    const int numberOfRowsToAdd = 100; // Example large number of rows
    clock_t start, end;
    double query_time_used;

    FILE* file = fopen("queryRowsPerformance.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "RowNumber,TimeInSeconds\n");

    for (int i = 0; i < numberOfRowsToAdd; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "Sample Name"}};
        
        addRow(db, "PerfTestTable", rowData);

        // Measure query time
        start = clock();
        queryRows(db, "PerfTestTable", queryAllPredicate);
        end = clock();
        query_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

        fprintf(file, "%d,%f\n", i + 1, query_time_used);
    }

    printf("Performance data recorded in 'queryRowsPerformance.csv'\n");

    // Close the CSV file
    fclose(file);

    // Clean up
    closeDatabase(db);
    remove("PerfTestDB.db");
}

bool updateAllPredicate(Database* db, const Row* row) {
    // Predicate that matches all rows
    return true;
}

void testUpdateRowsPerformance() {
    printf("Testing performance of updateRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "PerfTestDB", "PerfTestTable");

    const int numberOfRowsToAdd = 100; // Example large number of rows
    clock_t start, end;
    double update_time_used;

    FILE* file = fopen("updateRowsPerformance.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "RowNumber,TimeInSeconds\n");

    Data newData[2] = {{INTEGER, .value.intValue = 999}, {STRING, .value.strValue = "Updated Name"}};

    for (int i = 0; i < numberOfRowsToAdd; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "Sample Name"}};
        addRow(db, "PerfTestTable", rowData);

        // Measure update time
        start = clock();
        updateRows(db, "PerfTestTable", updateAllPredicate, newData);
        end = clock();
        update_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

        fprintf(file, "%d,%f\n", i + 1, update_time_used);
    }

    printf("Performance data recorded in 'updateRowsPerformance.csv'\n");

    fclose(file);

    closeDatabase(db);
    remove("PerfTestDB.db");
}

static int globalIDToDelete;

bool deleteSpecificIDPredicate(Database* db, const Row* row) {
    Data* data = getDataByIndex(db, "PerfTestTable", row, 0); // Assuming first column is ID
    bool result = (data != NULL && data->type == INTEGER && data->value.intValue == globalIDToDelete);
    return result;
}


void testDeleteRowsPerformance() {
    printf("Testing performance of deleteRows...\n");

    Database* db;
    setupDatabaseForTesting(&db, "PerfTestDB", "PerfTestTable");

    const int numberOfRows = 100; // Number of rows to add initially
    clock_t start, end;
    double delete_time_used;

    FILE* file = fopen("deleteRowsPerformance.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "RowNumber,TimeInSeconds\n");

    // Initially add 10000 rows
    for (int i = 0; i < numberOfRows; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "Sample Name"}};
        addRow(db, "PerfTestTable", rowData);
    }

    // Delete rows one by one in descending order
    for (int idToDelete = numberOfRows - 1; idToDelete >= 0; --idToDelete) {
        globalIDToDelete = idToDelete; 
        start = clock();
        deleteRows(db, "PerfTestTable", deleteSpecificIDPredicate);
        end = clock();
        delete_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

        fprintf(file, "%d,%f\n", idToDelete, delete_time_used);
    }

    printf("Performance data recorded in 'deleteRowsPerformance.csv'\n");

    // Close the CSV file
    fclose(file);

    // Clean up
    closeDatabase(db);
    remove("PerfTestDB.db");
}

long getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

void testFileSizeChanges() {
    printf("Testing file size changes during add and delete operations...\n");

    Database* db;
    setupDatabaseForTesting(&db, "PerfTestDB", "PerfTestTable");

    FILE* file = fopen("fileSizeChanges.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "RowNumber,FileSizeBytes\n");

    // Add 100 rows
    for (int i = 0; i < 100; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "Sample Name"}};
        addRow(db, "PerfTestTable", rowData);
        fprintf(file, "%d,%ld\n", i, getFileSize("PerfTestDB.db"));
    }

    // Delete 50 rows
    for (int i = 0; i < 50; ++i) {
        globalIDToDelete = i;
        deleteRows(db, "PerfTestTable", deleteSpecificIDPredicate);
        fprintf(file, "%d,%ld\n", i+100, getFileSize("PerfTestDB.db"));
    }

    // Add another 100 rows
    for (int i = 50; i < 150; ++i) {
        Data rowData[2] = {{INTEGER, .value.intValue = i}, {STRING, .value.strValue = "New Sample Name"}};
        addRow(db, "PerfTestTable", rowData);
        fprintf(file, "%d,%ld\n", i+150, getFileSize("PerfTestDB.db"));
    }

    printf("File size data recorded in 'fileSizeChanges.csv'\n");

    fclose(file);

    // Clean up
    closeDatabase(db);
    remove("PerfTestDB.db");
}

int main() {
    testAddRowPerformance();
    testQueryRowsPerformance();
    testUpdateRowsPerformance();
    testDeleteRowsPerformance();
    testFileSizeChanges();
    return 0;
}
