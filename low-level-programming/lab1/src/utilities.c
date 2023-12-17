#include "../include/utilities.h"

// Helper function to write individual data items to file
void writeDataToFile(FILE *file, const Data *data) {
    switch (data->type) {
        case DT_INT:
            fprintf(file, "%d", data->int_val);
            break;
        case DT_FLOAT:
            fprintf(file, "%f", data->float_val);
            break;
        case DT_STRING:
            fprintf(file, "%s", data->string_val);
            break;
        case DT_BOOL:
            fprintf(file, "%d", data->bool_val);
            break;
        // Handle other data types if necessary
    }
}

void writeDatabaseToFile(const Database *db) {
    FILE *file = fopen(db->filename, "w");
    if (!file) {
        perror("Unable to open the database file for writing");
        return;
    }

    // Write database metadata: database name and number of tables
    fprintf(file, "Database: %s\n", db->filename);
    fprintf(file, "Tables: %d\n", db->tableCount);

    // Iterate over each table and write its metadata and data
    for (int i = 0; i < db->tableCount; i++) {
        Table *table = db->tables[i];
        fprintf(file, "Table: %s, Columns: %d\n", table->name, table->numColumns);

        // Write column metadata
        for (int j = 0; j < table->numColumns; j++) {
            fprintf(file, "Column: %s, Type: %d\n", table->columns[j].name, table->columns[j].dataType);
        }

        // Write row data
        TableRow *currentRow = table->rows;
        while (currentRow) {
            for (int k = 0; k < table->numColumns; k++) {
                writeDataToFile(file, &currentRow->data[k]);
                if (k < table->numColumns - 1) fprintf(file, ", ");
            }
            fprintf(file, "\n");
            currentRow = currentRow->next;
        }
    }

    fclose(file);
}

TableRow *readRow(FILE *file, TableColumn *columns, int numColumns) {
    Data *rowData = malloc(sizeof(Data) * numColumns);

    for (int i = 0; i < numColumns; i++) {
        switch (columns[i].dataType) {
            case DT_INT: // Integer data type
                fscanf(file, "%d", &rowData[i].int_val);
                break;
            case DT_FLOAT: // Float data type
                fscanf(file, "%f", &rowData[i].float_val);
                break;
            case DT_STRING: { // String data type
                char buffer[256];
                fscanf(file, "%s", buffer);
                rowData[i].string_val = strdup(buffer);
                break;
            }
            case DT_BOOL: {// Boolean data type
                int boolVal;
                fscanf(file, "%d", &boolVal);
                rowData[i].bool_val = (bool)boolVal;
                break;
            }
            // Add more cases for other data types if needed
        }
        rowData[i].type = columns[i].dataType;
    }

    TableRow *row = malloc(sizeof(TableRow));
    row->data = rowData;
    row->next = NULL;
    return row;
}


Table *readTable(FILE *file) {
    char tableName[100];
    int numColumns, numRecords;
    fscanf(file, "%s %d %d", tableName, &numColumns, &numRecords);
    
    Table *table = malloc(sizeof(Table));
    table->name = strdup(tableName);
    table->numColumns = numColumns;
    table->columns = malloc(sizeof(TableColumn) * numColumns);
    table->rows = NULL;

    // Read column metadata
    for (int i = 0; i < numColumns; i++) {
        char columnName[100];
        int dataType;
        fscanf(file, "%s %d", columnName, &dataType);
        table->columns[i].name = strdup(columnName);
        table->columns[i].dataType = dataType;
    }

    // Read table data
    TableRow *lastRow = NULL;
    for (int i = 0; i < numRecords; i++) {
        TableRow *row = readRow(file, table->columns, numColumns);
        if (lastRow) {
            lastRow->next = row;
        } else {
            table->rows = row;
        }
        lastRow = row;
    }

    return table;
}

// Helper function to free a table row
void freeTableRow(TableRow *row) {
    if (row) {
        free(row->data); // Assuming 'data' is dynamically allocated
        free(row);
    }
}

// Helper function to free a table
void freeTable(Table *table) {
    if (table) {
        free(table->name);
        // for (int i = 0; i < table->numColumns; i++) {
        //     free(table->columns[i].name);
        // }
        free(table->columns);

        // Free each row
        TableRow *current = table->rows;
        while (current) {
            TableRow *temp = current;
            current = current->next;
            freeTableRow(temp);
        }

        free(table);
    }
}

Table *getTable(Database *db, const char *tableName) {
    if (db == NULL || tableName == NULL) {
        return NULL; 
    }

    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i]->name, tableName) == 0) {
            return db->tables[i]; 
        }
    }

    return NULL;
}