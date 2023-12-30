#include "../include/operations.h"
#include "../include/types.h"
#include "../include/utilities.h"
#include <string.h>

Database* createDatabase(const char *databaseName) {
    if (databaseName == NULL) {
        fprintf(stderr, "Error: Invalid database name.\n");
        return NULL;
    }

    Database *db = (Database *)malloc(sizeof(Database));
    if (db == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Database.\n");
        return NULL;
    }

    db->metadata.databaseName.length = strlen(databaseName);
    db->metadata.databaseName.value = (char *)malloc(db->metadata.databaseName.length + 1);
    if (db->metadata.databaseName.value == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for database name.\n");
        free(db);
        return NULL;
    }
    strcpy(db->metadata.databaseName.value, databaseName);
    db->metadata.tableCount = 0;
    db->metadata.firstTableOffset = -1;

    char fileName[256];
    sprintf(fileName, "%s.db", databaseName);
    db->dbConnection = fopen(fileName, "wb+");
    if (db->dbConnection == NULL) {
        fprintf(stderr, "Error: Unable to create or open database file.\n");
        free(db->metadata.databaseName.value);
        free(db);
        return NULL;
    }
    fclose(db->dbConnection);
    db->dbConnection = fopen(fileName, "rb+");

    db->node.offset = 0;
    db->node.prevOffset = -1;
    db->node.nextOffset = -1;
    db->node.prevNode = NULL;
    db->node.nextNode = NULL;

    serializeDatabase(db);


    return db;
}

Database* openDatabase(const char *databaseName) {
    if (databaseName == NULL) {
        fprintf(stderr, "Error: Invalid database name.\n");
        return NULL;
    }

    char fileName[256];
    sprintf(fileName, "%s.db", databaseName);

    FILE *file = fopen(fileName, "rb+");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open database file.\n");
        return NULL;
    }

    Database *db = (Database *)malloc(sizeof(Database));
    if (db == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Database.\n");
        return NULL;
    }

    db->dbConnection = file;

    Database *deserializedDb = deserializeDatabase(db, 0);
    if (deserializedDb == NULL) {
        fprintf(stderr, "Error: Failed to deserialize the database.\n");
        free(db);
        return NULL;
    }

    free(db); // Free the temporary database object

    return deserializedDb;
}

void closeDatabase(Database *db) {
    if (db == NULL) {
        fprintf(stderr, "Error: Invalid Database object.\n");
        return;
    }

    if (db->dbConnection != NULL) {
        fclose(db->dbConnection);
    }

    if (db->metadata.databaseName.value != NULL) {
        free(db->metadata.databaseName.value);
    }

    free(db);
}

void dropDatabase(const char *databaseName) {
    if (databaseName == NULL) {
        fprintf(stderr, "Error: Invalid database name.\n");
        return;
    }

    char fileName[256];
    sprintf(fileName, "%s.db", databaseName);

    if (remove(fileName) != 0) {
        fprintf(stderr, "Error: Failed to delete database file.\n");
    }
}

void createTable(Database *db, const char *tableName) {
    if (db == NULL || tableName == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for createTable.\n");
        return;
    }

    // Allocate memory for the new table
    Table *newTable = (Table *)malloc(sizeof(Table));
    if (newTable == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Table.\n");
        return;
    }

    // Initialize table metadata
    newTable->metadata.tableName.length = strlen(tableName);
    newTable->metadata.tableName.value = (char *)malloc(newTable->metadata.tableName.length + 1);
    if (newTable->metadata.tableName.value == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for table name.\n");
        free(newTable);
        return;
    }
    strcpy(newTable->metadata.tableName.value, tableName);
    newTable->metadata.columnCount = 0;
    newTable->metadata.recordCount = 0;
    newTable->metadata.firstRowOffset = -1;
    newTable->metadata.firstColumnOffset = -1;

    // Initialize the new table node
    fseek(db->dbConnection, 0, SEEK_END);
    newTable->node.offset = ftell(db->dbConnection);
    newTable->node.prevOffset = -1; // New table is at the beginning
    newTable->node.nextOffset = db->metadata.firstTableOffset; // Next is the current first table
    newTable->node.prevNode = NULL;
    newTable->node.nextNode = NULL;

    // Serialize the new table (this should update newTable->node.offset)
    serializeTable(db, newTable);

    // Get the current first table and update its previous link
    Table *currentFirstTable = getFirstTable(db);
    if (currentFirstTable != NULL) {
        currentFirstTable->node.prevOffset = newTable->node.offset;

        // Link newTable and currentFirstTable
        newTable->node.nextNode = (Node *) currentFirstTable;
        currentFirstTable->node.prevNode = (Node *) newTable;

        // serializeTable(db, currentFirstTable);
    }

    // Update the database's firstTableOffset and link the new first table
    db->metadata.firstTableOffset = newTable->node.offset;
    db->firstTable = (Node *) newTable;

    // Serialize the updated database metadata
    serializeDatabase(db);

    // Free allocated memory
    // free(newTable->metadata.tableName.value);
    // free(newTable);
}

void addColumn(Database *db, const char *tableName, ColumnMetadata columnMetadata) {
    if (db == NULL || tableName == NULL) {
        fprintf(stderr, "Error: Invalid parameters for addColumn.\n");
        return;
    }

    // Get the specified table
    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return;
    }

    // // Allocate memory for the new column
    Column *newColumn = (Column *)malloc(sizeof(Column));
    if (newColumn == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Column.\n");
        return;
    }

    // // Initialize column metadata
    // newColumn->metadata = columnMetadata;
    newColumn->metadata.columnName.value = strdup(columnMetadata.columnName.value);
    newColumn->metadata.columnName.length = strlen(columnMetadata.columnName.value);
    newColumn->metadata.dataType = columnMetadata.dataType;
    // printf("%s\n", newColumn->metadata.columnName.value);

    // // Initialize the new column node
    fseek(db->dbConnection, 0, SEEK_END);
    newColumn->node.offset = ftell(db->dbConnection);
    newColumn->node.prevOffset = -1; // New column is at the beginning
    newColumn->node.nextOffset = table->metadata.firstColumnOffset; // Next is the current first column
    newColumn->node.prevNode = NULL;
    newColumn->node.nextNode = NULL;

    // // Serialize the new column (this should update newColumn->node.offset)
    serializeColumn(db, newColumn);

    // Get the current first column and update its previous link
    Column *currentFirstColumn = getFirstColumn(db, tableName);
    if (currentFirstColumn != NULL) {
        currentFirstColumn->node.prevOffset = newColumn->node.offset;

        // Link newColumn and currentFirstColumn
        newColumn->node.nextNode = (Node *) currentFirstColumn;
        currentFirstColumn->node.prevNode = (Node *)newColumn;

        serializeColumn(db, currentFirstColumn);
    }

    // Update the table's firstColumnOffset and increment the column count
    table->metadata.firstColumnOffset = newColumn->node.offset;
    table->metadata.columnCount++;
    table->firstColumn = (Node *) newColumn;

    // Serialize the updated table metadata
    serializeTable(db, table);

    // Free allocated memory
    // free(newColumn);
}

void addRow(Database *db, const char *tableName, Data *data) {
    if (db == NULL || tableName == NULL || data == NULL) {
        fprintf(stderr, "Error: Invalid parameters for addRow.\n");
        return;
    }

    // Get the specified table
    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return;
    }

    if (table->metadata.columnCount == 0) {
        fprintf(stderr, "Error: No columns in the table to add data.\n");
        return;
    }

    // Add an empty row to the specified table
    Row *newrow = addEmptyRow(db, tableName);
    if (newrow == NULL) {
        fprintf(stderr, "Error: Unable to add a new row to the table.\n");
        return;
    }

    // Iterate through the columns to set data types for each data element
    Column *currentColumn = getFirstColumn(db, tableName);
    for (int i = 0; i < table->metadata.columnCount; i++) {
        if (currentColumn == NULL) {
            fprintf(stderr, "Error: Mismatch in the number of columns and data.\n");
            return;
        }

        data[table->metadata.columnCount-i-1].type = currentColumn->metadata.dataType;
        Column *nextColumn = getNextColumn(db, currentColumn);
        free(currentColumn); // Clean up the current column
        currentColumn = nextColumn;
    }

    // Add data items to the new row
    for (int i = 0; i < table->metadata.columnCount; i++) {
        addData(db, newrow, &data[i]);
    }
}

RowNode* queryRows(Database *db, const char *tableName, PredicateFunction predicate, PredicateContext* context) {
    if (db == NULL || tableName == NULL || predicate == NULL) {
        return NULL;
    }

    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        return NULL;
    }

    RowNode *head = NULL;
    RowNode *tail = NULL;

    Row *currentRow = getFirstRow(db, tableName);
    while (currentRow != NULL) {
        if (predicate(db, currentRow, NULL)) {
            RowNode *newNode = (RowNode *)malloc(sizeof(RowNode));
            newNode->row = currentRow;
            newNode->next = NULL;

            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        if (currentRow->node.nextOffset == -1) break;
        Row *nextRow = deserializeRow(db, currentRow->node.nextOffset);
        currentRow = nextRow;
    }

    return head;
}

void deleteRows(Database *db, const char *tableName, PredicateFunction predicate, PredicateContext* context) {
    if (db == NULL || tableName == NULL || predicate == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deleteRows.\n");
        return;
    }

    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return;
    }

    Row *currentRow = getFirstRow(db, tableName);
    while (currentRow != NULL) {
        Row *nextRow = getNextRow(db, currentRow);

        // Check if the row satisfies the predicate
        if (predicate(db, currentRow, context)) {
            deleteRow(db, tableName, currentRow);
        }

        // Move to the next row
        currentRow = nextRow;
    }
}

void updateRows(Database *db, const char *tableName, PredicateFunction predicate, Data *newData, PredicateContext* context) {
    if (db == NULL || tableName == NULL || predicate == NULL || newData == NULL) {
        fprintf(stderr, "Error: Invalid parameters for updateRows.\n");
        return;
    }

    Table *table = getTableByName(db, tableName);
    if (table == NULL) {
        fprintf(stderr, "Error: Table not found.\n");
        return;
    }

    Row *currentRow = getFirstRow(db, tableName);
    while (currentRow != NULL) {
        Row *nextRow = getNextRow(db, currentRow);

        if (predicate(db, currentRow, context)) {
            // Create a new row with the updated data
            Row *updatedRow = (Row *)malloc(sizeof(Row));
            if (updatedRow == NULL) {
                fprintf(stderr, "Error: Memory allocation failed for Row.\n");
                return;
            }

            // Initialize the new row node
            fseek(db->dbConnection, 0, SEEK_END);
            updatedRow->node.offset = ftell(db->dbConnection);
            updatedRow->node.prevOffset = -1;
            updatedRow->node.nextOffset = -1; 
            updatedRow->node.prevNode = NULL;
            updatedRow->node.nextNode = NULL;
            updatedRow->firstData = NULL; // No data in the new row yet

            // Initialize row metadata
            updatedRow->metadata.firstDataOffset = -1;

            // Serialize the new row (this should update newrow->node.offset)
            serializeRow(db, updatedRow);

            // Iterate through the columns to set data types for each data element
            Column *currentColumn = getFirstColumn(db, tableName);
            for (int i = 0; i < table->metadata.columnCount; i++) {
                if (currentColumn == NULL) {
                    fprintf(stderr, "Error: Mismatch in the number of columns and data.\n");
                    return;
                }

                newData[table->metadata.columnCount-i-1].type = currentColumn->metadata.dataType;
                Column *nextColumn = getNextColumn(db, currentColumn);
                free(currentColumn); // Clean up the current column
                currentColumn = nextColumn;
            }

            // Add new data to the updated row
            for (int i = 0; i < table->metadata.columnCount; i++) {
                addData(db, updatedRow, &newData[i]);
            }

            // Relink adjacent rows
            Row *prevRow = getPrevRow(db, currentRow);
            if (prevRow != NULL) {
                prevRow->node.nextOffset = updatedRow->node.offset;
                serializeRow(db, prevRow);
                free(prevRow);
            } else {
                table->metadata.firstRowOffset = updatedRow->node.offset;
                serializeTable(db, table);
            }

            if (nextRow != NULL) {
                nextRow->node.prevOffset = updatedRow->node.offset;
                serializeRow(db, nextRow);
            }

            // Delete the original row
            // deleteRow(db, tableName, currentRow);
        }

        currentRow = nextRow;
    }
}







