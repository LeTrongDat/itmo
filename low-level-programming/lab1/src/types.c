#include "types.h"
#include "utilities.h"
#include <stdlib.h>

void prependNode(Node *currentNode, Node *newNode) {
    newNode->nextNode = currentNode;
    newNode->prevNode = NULL;
    currentNode->prevNode = newNode;

    newNode->prevOffset = -1;
    newNode->nextOffset = currentNode->offset;
    currentNode->prevOffset = newNode->offset;
}

void removeNode(Node *node) {
    if (node->prevNode) {
        node->prevNode->nextNode = node->nextNode;
        node->prevNode->nextOffset = node->nextOffset;
    }
    if (node->nextNode) {
        node->nextNode->prevNode = node->prevNode;
        node->nextNode->prevOffset = node->prevOffset;
    }
}

void serializeNode(Database *db, Node *node) {
    if (db == NULL || db->dbConnection == NULL || node == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeNode.\n");
        return;
    }

    fseek(db->dbConnection, node->offset, SEEK_SET);
    fwrite(&node->offset, sizeof(node->offset), 1, db->dbConnection);
    fwrite(&node->prevOffset, sizeof(node->prevOffset), 1, db->dbConnection);
    fwrite(&node->nextOffset, sizeof(node->nextOffset), 1, db->dbConnection);

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write to database file.\n");
    }
}

void serializeDatabase(Database *db) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeDatabase.\n");
        return;
    }

    serializeNode(db, &db->node);

    fwrite(&db->metadata.databaseName.length, sizeof(db->metadata.databaseName.length), 1, db->dbConnection);
    fwrite(db->metadata.databaseName.value, sizeof(char), db->metadata.databaseName.length, db->dbConnection);
    fwrite(&db->metadata.tableCount, sizeof(db->metadata.tableCount), 1, db->dbConnection);
    fwrite(&db->metadata.firstTableOffset, sizeof(db->metadata.firstTableOffset), 1, db->dbConnection);

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write database metadata to file.\n");
    }
}

void serializeTable(Database *db, Table *table) {
    if (db == NULL || db->dbConnection == NULL || table == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeTable.\n");
        return;
    }

    serializeNode(db, &table->node);

    fwrite(&table->metadata.tableName.length, sizeof(table->metadata.tableName.length), 1, db->dbConnection);
    fwrite(table->metadata.tableName.value, sizeof(char), table->metadata.tableName.length, db->dbConnection);
    fwrite(&table->metadata.columnCount, sizeof(table->metadata.columnCount), 1, db->dbConnection);
    fwrite(&table->metadata.recordCount, sizeof(table->metadata.recordCount), 1, db->dbConnection);
    fwrite(&table->metadata.firstRowOffset, sizeof(table->metadata.firstRowOffset), 1, db->dbConnection);
    fwrite(&table->metadata.firstColumnOffset, sizeof(table->metadata.firstColumnOffset), 1, db->dbConnection);

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write table metadata to file.\n");
    }
}

void serializeColumn(Database *db, Column *column) {
    if (db == NULL || db->dbConnection == NULL || column == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeColumn.\n");
        return;
    }

    serializeNode(db, &column->node);

    fwrite(&column->metadata.columnName.length, sizeof(column->metadata.columnName.length), 1, db->dbConnection);
    fwrite(column->metadata.columnName.value, sizeof(char), column->metadata.columnName.length, db->dbConnection);
    fwrite(&column->metadata.dataType, sizeof(column->metadata.dataType), 1, db->dbConnection);

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write column metadata to file.\n");
    }
}

void serializeRow(Database *db, Row *row) {
    if (db == NULL || db->dbConnection == NULL || row == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeRow.\n");
        return;
    }

    serializeNode(db, &row->node);

    fwrite(&row->metadata.firstDataOffset, sizeof(row->metadata.firstDataOffset), 1, db->dbConnection);

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write row metadata to file.\n");
    }
}

void serializeData(Database *db, Data *data) {
    if (db == NULL || db->dbConnection == NULL || data == NULL) {
        fprintf(stderr, "Error: Invalid parameters for serializeData.\n");
        return;
    }

    serializeNode(db, &data->node);

    fwrite(&data->type, sizeof(data->type), 1, db->dbConnection);
    fwrite(&data->dataSize, sizeof(data->dataSize), 1, db->dbConnection);

    switch (data->type) {
        case STRING:
            fwrite(data->value.strValue, sizeof(char), data->dataSize, db->dbConnection);
            break;
        case BOOLEAN:
            fwrite(&data->value.boolValue, sizeof(data->value.boolValue), 1, db->dbConnection);
            break;
        case INTEGER:
            fwrite(&data->value.intValue, sizeof(data->value.intValue), 1, db->dbConnection);
            break;
        case FLOAT:
            fwrite(&data->value.floatValue, sizeof(data->value.floatValue), 1, db->dbConnection);
            break;
        default:
            fprintf(stderr, "Error: Unknown data type.\n");
            return;
    }

    if (fflush(db->dbConnection) != 0) {
        fprintf(stderr, "Error: Failed to write data to database file.\n");
    }
}

Node* deserializeNode(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeNode.\n");
        return NULL;
    }

    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Node.\n");
        return NULL;
    }

    fseek(db->dbConnection, offset, SEEK_SET);

    fread(&node->offset, sizeof(node->offset), 1, db->dbConnection);
    fread(&node->prevOffset, sizeof(node->prevOffset), 1, db->dbConnection);
    fread(&node->nextOffset, sizeof(node->nextOffset), 1, db->dbConnection);

    node->prevNode = NULL;
    node->nextNode = NULL;

    return node;
}

Database* deserializeDatabase(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeDatabase.\n");
        return NULL;
    }

    Database *newDb = (Database *)malloc(sizeof(Database));
    if (newDb == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Database.\n");
        return NULL;
    }

    Node *node = deserializeNode(db, offset);
    if (node == NULL) {
        free(newDb);
        return NULL;
    }
    newDb->node = *node;
    free(node);

    fread(&newDb->metadata.databaseName.length, sizeof(newDb->metadata.databaseName.length), 1, db->dbConnection);
    newDb->metadata.databaseName.value = (char *)malloc(newDb->metadata.databaseName.length + 1);
    if (newDb->metadata.databaseName.value == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for database name.\n");
        free(newDb);
        return NULL;
    }
    fread(newDb->metadata.databaseName.value, sizeof(char), newDb->metadata.databaseName.length, db->dbConnection);
    newDb->metadata.databaseName.value[newDb->metadata.databaseName.length] = '\0';

    fread(&newDb->metadata.tableCount, sizeof(newDb->metadata.tableCount), 1, db->dbConnection);
    fread(&newDb->metadata.firstTableOffset, sizeof(newDb->metadata.firstTableOffset), 1, db->dbConnection);

    newDb->dbConnection = db->dbConnection;

    return newDb;
}

Table* deserializeTable(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeTable.\n");
        return NULL;
    }

    Table *table = (Table *)malloc(sizeof(Table));
    if (table == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Table.\n");
        return NULL;
    }

    Node *node = deserializeNode(db, offset);
    if (node == NULL) {
        free(table);
        return NULL;
    }
    table->node = *node;
    free(node);

    fread(&table->metadata.tableName.length, sizeof(table->metadata.tableName.length), 1, db->dbConnection);
    table->metadata.tableName.value = (char *)malloc(table->metadata.tableName.length + 1);
    if (table->metadata.tableName.value == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for table name.\n");
        free(table);
        return NULL;
    }
    fread(table->metadata.tableName.value, sizeof(char), table->metadata.tableName.length, db->dbConnection);
    table->metadata.tableName.value[table->metadata.tableName.length] = '\0';

    fread(&table->metadata.columnCount, sizeof(table->metadata.columnCount), 1, db->dbConnection);
    fread(&table->metadata.recordCount, sizeof(table->metadata.recordCount), 1, db->dbConnection);
    fread(&table->metadata.firstRowOffset, sizeof(table->metadata.firstRowOffset), 1, db->dbConnection);
    fread(&table->metadata.firstColumnOffset, sizeof(table->metadata.firstColumnOffset), 1, db->dbConnection);

    return table;
}

Column* deserializeColumn(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeColumn.\n");
        return NULL;
    }

    Column *column = (Column *)malloc(sizeof(Column));
    if (column == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Column.\n");
        return NULL;
    }

    Node *node = deserializeNode(db, offset);
    if (node == NULL) {
        free(column);
        return NULL;
    }
    column->node = *node;
    free(node);

    fread(&column->metadata.columnName.length, sizeof(column->metadata.columnName.length), 1, db->dbConnection);
    column->metadata.columnName.value = (char *)malloc(column->metadata.columnName.length + 1);
    if (column->metadata.columnName.value == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for column name.\n");
        free(column);
        return NULL;
    }
    fread(column->metadata.columnName.value, sizeof(char), column->metadata.columnName.length, db->dbConnection);
    column->metadata.columnName.value[column->metadata.columnName.length] = '\0';

    fread(&column->metadata.dataType, sizeof(column->metadata.dataType), 1, db->dbConnection);

    return column;
}

Row* deserializeRow(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeRow.\n");
        return NULL;
    }

    Row *row = (Row *)malloc(sizeof(Row));
    if (row == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Row.\n");
        return NULL;
    }

    Node *node = deserializeNode(db, offset);
    if (node == NULL) {
        free(row);
        return NULL;
    }
    row->node = *node;
    free(node);

    fread(&row->metadata.firstDataOffset, sizeof(row->metadata.firstDataOffset), 1, db->dbConnection);

    return row;
}

Data* deserializeData(Database *db, long offset) {
    if (db == NULL || db->dbConnection == NULL) {
        fprintf(stderr, "Error: Invalid parameters for deserializeData.\n");
        return NULL;
    }

    Data *data = (Data *)malloc(sizeof(Data));
    if (data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Data.\n");
        return NULL;
    }

    Node *node = deserializeNode(db, offset);
    if (node == NULL) {
        free(data);
        return NULL;
    }
    data->node = *node;
    free(node);

    fread(&data->type, sizeof(data->type), 1, db->dbConnection);
    fread(&data->dataSize, sizeof(data->dataSize), 1, db->dbConnection);

    switch (data->type) {
        case STRING:
            data->value.strValue = (char *)malloc(data->dataSize + 1);
            if (data->value.strValue == NULL) {
                fprintf(stderr, "Error: Memory allocation failed for string value.\n");
                free(data);
                return NULL;
            }
            fread(data->value.strValue, sizeof(char), data->dataSize, db->dbConnection);
            data->value.strValue[data->dataSize] = '\0';
            break;
        case BOOLEAN:
            fread(&data->value.boolValue, sizeof(data->value.boolValue), 1, db->dbConnection);
            break;
        case INTEGER:
            fread(&data->value.intValue, sizeof(data->value.intValue), 1, db->dbConnection);
            break;
        case FLOAT:
            fread(&data->value.floatValue, sizeof(data->value.floatValue), 1, db->dbConnection);
            break;
        default:
            fprintf(stderr, "Error: Unknown data type encountered in deserializeData.\n");
            free(data);
            return NULL;
    }

    return data;
}

char* dataTypeToString(DataType dataType) {
    switch (dataType) {
        case STRING: return "STRING";
        case BOOLEAN: return "BOOLEAN";
        case INTEGER: return "INTEGER";
        case FLOAT: return "FLOAT";
        default: return "UNKNOWN";
    }
}


char* toJSONTable(Database *db, const char *tableName, RowNode *rowNode, char **selectedColumns) {
    if (!db || !tableName) return NULL;

    Table *table = getTableByName(db, tableName);
    if (!table) return NULL;

    int buffer_size = 1024;
    char *result = malloc(buffer_size);
    if (!result) return NULL;
    int length = 0;

    length += snprintf(result + length, buffer_size - length, "{\n  \"tableName\": \"%s\",\n  \"columns\": [", 
                       table->metadata.tableName.value);

    Column *currentColumn = getFirstColumn(db, tableName); 
    while (currentColumn) {
        if (length >= buffer_size - 200) {
            buffer_size *= 2;
            result = realloc(result, buffer_size);
            if (!result) return NULL;
        }

        length += snprintf(result + length, buffer_size - length, 
                           "\n    {\"columnName\": \"%s\", \"dataType\": \"%s\"}", 
                           currentColumn->metadata.columnName.value, 
                           dataTypeToString(currentColumn->metadata.dataType));

        currentColumn = getNextColumn(db, currentColumn);
        if (currentColumn) {
            length += snprintf(result + length, buffer_size - length, ",");
        }
    }

    length += snprintf(result + length, buffer_size - length, "\n  ],\n  \"rows\": [");

    while (rowNode) {
        Row *currentRow = rowNode->row;
        if (length >= buffer_size - 200) {
            buffer_size *= 2;
            result = realloc(result, buffer_size);
            if (!result) return NULL;
        }

        length += snprintf(result + length, buffer_size - length, "\n    {");
        for (int i = 0; selectedColumns[i] != NULL; i++) {
            int columnIndex = isColumnExists(db, table->metadata.tableName.value, selectedColumns[i]);
            if (columnIndex < 0) continue;

            Data *data = getDataByIndex(db, table->metadata.tableName.value, currentRow, columnIndex);
            Column *column = getColumnByIndex(db, table, columnIndex);
            if (!data || !column) continue;

            switch (column->metadata.dataType) {
                case STRING:
                    length += snprintf(result + length, buffer_size - length, "\"%s\": \"%s\"", selectedColumns[i], data->value.strValue);
                    break;
                case BOOLEAN:
                    length += snprintf(result + length, buffer_size - length, "\"%s\": %s", selectedColumns[i], data->value.boolValue ? "true" : "false");
                    break;
                case INTEGER:
                    length += snprintf(result + length, buffer_size - length, "\"%s\": %d", selectedColumns[i], data->value.intValue);
                    break;
                case FLOAT:
                    length += snprintf(result + length, buffer_size - length, "\"%s\": %f", selectedColumns[i], data->value.floatValue);
                    break;
                default:
                    length += snprintf(result + length, buffer_size - length, "\"%s\": null", selectedColumns[i]);
            }

            if (selectedColumns[i + 1] != NULL) {
                length += snprintf(result + length, buffer_size - length, ", ");
            }
        }
        length += snprintf(result + length, buffer_size - length, "}");

        rowNode = rowNode->next;
        if (rowNode) {
            length += snprintf(result + length, buffer_size - length, ",");
        }
    }


    length += snprintf(result + length, buffer_size - length, "\n  ]\n}");

    return result;
}







