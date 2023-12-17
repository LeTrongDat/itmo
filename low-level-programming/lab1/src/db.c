#include "../include/db.h"
#include "../include/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Database *initDatabase(const char *filename) {
    Database *db = malloc(sizeof(Database));
    db->filename = strdup(filename);
    db->tables = NULL;
    db->tableCount = 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open the database file");
        free(db);
        return NULL;
    }

    // Read database metadata
    char dbName[100];
    fscanf(file, "%s %d", dbName, &db->tableCount);
    db->tables = malloc(sizeof(Table *) * db->tableCount);

    for (int i = 0; i < db->tableCount; i++) {
        db->tables[i] = readTable(file);
    }

    fclose(file);
    return db;
}

Table *createTable(Database *db, const char *tableName, TableColumn *columns, int numColumns) {
    Table *table = malloc(sizeof(Table));
    table->name = strdup(tableName);
    table->columns = malloc(sizeof(TableColumn) * numColumns);
    memcpy(table->columns, columns, sizeof(TableColumn) * numColumns);
    table->numColumns = numColumns;
    table->rows = NULL;

    // Add the table to the database
    db->tableCount++;
    db->tables = realloc(db->tables, sizeof(Table *) * db->tableCount);
    db->tables[db->tableCount - 1] = table;

    writeDatabaseToFile(db); // Update the database file after creating a new table
    return table;
}

void dropTable(Database *db, const char *tableName) {
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i]->name, tableName) == 0) {
            freeTable(db->tables[i]);

            // Shift the remaining tables
            for (int j = i; j < db->tableCount - 1; j++) {
                db->tables[j] = db->tables[j + 1];
            }

            db->tableCount--;
            db->tables = realloc(db->tables, sizeof(Table *) * db->tableCount);
            
            writeDatabaseToFile(db); // Update the database file after dropping a table
            
            return;
        }
    }
}

void freeDatabase(Database *db) {
    if (db) {
        free(db->filename);
        // Free memory for each table
        for (int i = 0; i < db->tableCount; i++) {
            freeTable(db->tables[i]);
        }
        free(db->tables);
        free(db);
    }
}

Database *createDatabase(const char *filename) {
    Database *db = malloc(sizeof(Database));
    db->filename = strdup(filename);
    db->tables = NULL;
    db->tableCount = 0;

    writeDatabaseToFile(db);  // Create an initial empty file for the database
    return db;
}


// Insert data into a table
int insertData(Database *db, const char *tableName, Data *data, int numFields) {
    Table *table = getTable(db, tableName);
    // Check if table and data are valid
    if (!table || !data || numFields <= 0) {
        return -1;
    }

    // Create a new row and allocate memory for its data
    TableRow *newRow = malloc(sizeof(TableRow));
    if (!newRow) {
        return -1;
    }

    newRow->data = malloc(sizeof(Data) * numFields);
    if (!newRow->data) {
        free(newRow);
        return -1;
    }

    // Copy data into the new row
    for (int i = 0; i < numFields; i++) {
        newRow->data[i] = data[i];
    }

    // Insert the new row at the beginning of the table's row list
    newRow->next = table->rows;
    table->rows = newRow;

    writeDatabaseToFile(db);

    // Return 1 row affected
    return 1;
}

// Update data in a table based on a comparison function
int updateData(Database *db, const char *tableName, ComparisonFunc compFunc, Data *newData, int numFields) {
    Table *table = getTable(db, tableName);
    if (!table || !compFunc || !newData || numFields <= 0) {
        return -1;
    }

    int rowsAffected = 0;
    for (TableRow *row = table->rows; row != NULL; row = row->next) {
        if (compFunc(row->data)) {
            for (int i = 0; i < numFields; i++) {
                row->data[i] = newData[i];
            }
            rowsAffected++;
        }
    }

    // writeDatabaseToFile(db);

    return rowsAffected;
}

// Delete data from a table based on a comparison function
int deleteData(Database *db, const char *tableName, ComparisonFunc compFunc) {
    Table *table = getTable(db, tableName);
    if (!table || !compFunc) {
        return -1;
    }

    int rowsAffected = 0;
    TableRow *prev = NULL, *current = table->rows;

    while (current != NULL) {
        if (compFunc(current->data)) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->rows = current->next;
            }

            TableRow *toDelete = current;
            current = current->next;

            free(toDelete->data);
            free(toDelete);

            rowsAffected++;
        } else {
            prev = current;
            current = current->next;
        }
    }

    writeDatabaseToFile(db);

    return rowsAffected;
}

// Query data from a table based on a comparison function
int queryData(Database *db, const char *tableName, ComparisonFunc compFunc, Data **outDataArray) {
    Table *table = getTable(db, tableName);
    if (!table || !compFunc || !outDataArray) {
        return -1;
    }

    int rowsFound = 0;
    for (TableRow *row = table->rows; row != NULL; row = row->next) {
        if (compFunc(row->data)) {
            rowsFound++;
        }
    }

    if (rowsFound == 0) {
        return 0;
    }

    *outDataArray = malloc(sizeof(Data) * rowsFound * table->numColumns);
    if (!*outDataArray) {
        return -1;
    }

    int idx = 0;
    for (TableRow *row = table->rows; row != NULL; row = row->next) {
        if (compFunc(row->data)) {
            for (int i = 0; i < table->numColumns; i++) {
                (*outDataArray)[idx++] = row->data[i];
            }
        }
    }

    return rowsFound;
}