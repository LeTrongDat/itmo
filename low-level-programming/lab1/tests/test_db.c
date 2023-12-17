#include "../include/db.h"
#include "../include/data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void test_createDatabase() {
    Database *db = createDatabase("test_db.db");
    assert(db != NULL);
    assert(db->tables == NULL);
    assert(db->tableCount == 0);
    printf("Test createDatabase: Passed\n");
    freeDatabase(db);
}

void test_createAndDropTable() {
    Database *db = createDatabase("test_db.db");

    TableColumn columns[] = {
        {"ID", DT_INT},
        {"Value", DT_FLOAT},
        {"Description", DT_STRING},
        {"IsActive", DT_BOOL}
    };
    createTable(db, "TestTable", columns, 4);
    assert(db->tableCount == 1);
    printf("Test createTable: Passed\n");

    dropTable(db, "TestTable");
    assert(db->tableCount == 0);
    printf("Test dropTable: Passed\n");
    freeDatabase(db);
}

char *randomString(int length) {
    char *characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int charactersLength = strlen(characters);

    char *randomStr = malloc(length + 1); 

    if (randomStr) {
        for (int i = 0; i < length; i++) {
            int index = rand() % charactersLength;
            randomStr[i] = characters[index];
        }
        randomStr[length] = '\0';
    }

    return randomStr;
}

void setupDatabase(Database **db, const char *filename, const char *tableName) {
    *db = createDatabase(filename);
    TableColumn columns[] = {{"ID", DT_INT}, {"Value", DT_FLOAT}, {"Description", DT_STRING}};
    createTable(*db, tableName, columns, 3);

    for (int i = 0; i < 10; i++) {
        Data row[] = {
            {.type = DT_INT, .int_val = i},
            {.type = DT_FLOAT, .float_val = i * 10.0},
            {.type = DT_STRING, .string_val = randomString(20)} // 'randomString' needs to be implemented
        };
        insertData(*db, tableName, row, 3);
        // free(row[2].string_val); // Free the string allocated by randomString
    }
}

void cleanupDatabase(Database *db, const char *filename) {
    remove(filename);
    freeDatabase(db);
}

void test_insertData() {
    Database *db;
    const char *dbName = "test_insert.db";
    const char *tableName = "TestTable";

    // Setup database and table
    setupDatabase(&db, dbName, tableName);

    // Create data to insert
    Data row[] = {
        {.type = DT_INT, .int_val = 1},
        {.type = DT_FLOAT, .float_val = 100.5},
        {.type = DT_STRING, .string_val = "Test Description"},
        {.type = DT_BOOL, .bool_val = true}
    };

    // Insert data into table
    int result = insertData(db, tableName, row, 4);
    assert(result == 1);  // Check if one row was affected

    printf("Test insertData: Passed\n");

    // Cleanup
    cleanupDatabase(db, dbName);
}

bool updateCondition(Data *data) {
    return data[0].type == DT_INT && data[0].int_val > 5;
}

void test_updateData() {
    Database *db;
    const char *dbName = "test_update.db";
    const char *tableName = "TestTable";

    // Setup database and table with initial data
    setupDatabase(&db, dbName, tableName);

    // Create new data to update
    Data newData[] = {
        {.type = DT_INT, .int_val = 999},      // New ID
        {.type = DT_FLOAT, .float_val = 999.9},// New Value
        {.type = DT_STRING, .string_val = "Updated Description"},
        {.type = DT_BOOL, .bool_val = false}
    };

    // Update data in table where the condition is true
    int rowsAffected = updateData(db, tableName, updateCondition, newData, 4);
    assert(rowsAffected > 0);  // Check if at least one row was updated

    printf("Test updateData: Passed\n");

    // Cleanup
    cleanupDatabase(db, dbName);
}


int main() {
    test_createDatabase();
    test_createAndDropTable();
    test_insertData();
    test_updateData();
    printf("All tests completed.\n");
    return 0;
}
