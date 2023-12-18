#include "../include/db.h"

// Serialization function for a Database object
void serializeDatabase(FILE *file, const Database *db) {
    if (!file || !db) return;

    fseek(file, db->fileDb.offset, SEEK_SET);

    // Write the database name
    fwrite(db->databaseName, sizeof(db->databaseName), 1, file);

    // Write the FileDatabase data (offsets)
    fwrite(&(db->fileDb), sizeof(db->fileDb), 1, file);

    // Write the metadata (number of tables)
    fwrite(&(db->numTables), sizeof(db->numTables), 1, file);

    // Additional serialization logic for the Database can be added here
}

// Deserialization function for a Database object
void deserializeDatabase(FILE *file, Database *db) {
    if (!file || !db) return;

    fseek(file, db->fileDb.offset, SEEK_SET);

    // Read the database name
    fread(db->databaseName, sizeof(db->databaseName), 1, file);

    // Read the FileDatabase data (offsets)
    fread(&(db->fileDb), sizeof(db->fileDb), 1, file);

    // Read the metadata (number of tables)
    fread(&(db->numTables), sizeof(db->numTables), 1, file);

    // Additional deserialization logic for the Database can be added here
}

// Function to get the last Table in the Database
Table* getLastTable(Database *db) {
    if (!db) return NULL;
    if (db->lastTable) return db->lastTable;

    FILE* file = fopen(db->databaseName, "rb+");
    if (!file) {
        // File creation failed, cleanup and return NULL
        free(db);
        return NULL;
    }

    // Load from file if the lastTable is not in memory and file is provided
    if (db->fileDb.lastTableOffset != 0) {
        Table* lastTable = malloc(sizeof(Table));
        if (!lastTable) return NULL;
        if (lastTable->lastRow) {
        }
        fseek(file, db->fileDb.lastTableOffset, SEEK_SET);
        deserializeTable(file, lastTable);  // Assuming deserializeTable is implemented elsewhere
        db->lastTable = lastTable;
        return lastTable;
    }

    fclose(file);

    return NULL;
}

// Function to get a Table by name
Table* getTableByName(Database *db, const char *tableName) {
    if (!db || !tableName) return NULL;

    // printf("pre getlastrow %d\n", db->lastTable->fileTable.offset);
    if (db->lastTable->lastRow) printf("pre getlastrow 2.1 %d\n", db->lastTable->lastRow->lastData->fileData.offset);
    FILE* file = fopen(db->databaseName, "r+");

    if (!file) {
        // File creation failed, cleanup and return NULL
        free(db);
        return NULL;
    }

    // Iterate through the linked list of tables to find the matching name
    Table* currentTable = getLastTable(db);  // Start from the last table

    if (db->lastTable->lastRow) printf("pre getlastrow 2.2 %d\n", db->lastTable->lastRow->lastData->fileData.offset);
    while (currentTable) {
        if (strcmp(currentTable->tableName, tableName) == 0) {
            fclose(file);

            if (db->lastTable->lastRow) printf("pre getlastrow 2.3 %d\n", currentTable->lastRow->lastData->fileData.offset);
            return currentTable;
        }
        currentTable = getPrevTable(file, currentTable); // Move to the previous table
    }


    fclose(file);

    return NULL;
}

// Function to free a Database object
void freeDatabase(Database* db) {
    if (!db) return;

    // Free all tables in the database
    freeTables(db->lastTable);  // Assuming freeTables is implemented in table.c

    // Finally, free the Database object itself
    free(db);
}
