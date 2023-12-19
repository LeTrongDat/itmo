#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

typedef struct Node {
    long offset;
    long prevOffset;
    long nextOffset;
    struct Node *prevNode;
    struct Node *nextNode;
} Node;

typedef struct Name {
    int length;
    char *value;
} Name;

typedef enum { STRING, BOOLEAN, INTEGER, FLOAT } DataType;

typedef struct TableMetadata {
    Name tableName;
    int columnCount;
    int recordCount;
    long firstRowOffset;
    long firstColumnOffset;
} TableMetadata;

typedef struct ColumnMetadata {
    Name columnName;
    DataType dataType;
} ColumnMetadata;

typedef struct RowMetadata {
    long firstDataOffset;
} RowMetadata;

typedef struct DatabaseMetadata {
    Name databaseName;
    int tableCount;
    long firstTableOffset;
} DatabaseMetadata;



typedef struct Data {
    Node node;
    DataType type;
    int dataSize;
    union {
        char *strValue;
        int boolValue;
        int intValue;
        float floatValue;
    } value;
} Data;

typedef struct Row {
    Node node;
    Node *firstData;
    RowMetadata metadata;
} Row;

typedef struct Column {
    Node node;
    ColumnMetadata metadata;
} Column;

typedef struct Table {
    Node node;
    Node *firstColumn;
    Node *firstRow;
    TableMetadata metadata;
} Table;

typedef struct Database {
    Node node;
    Node *firstTable;
    DatabaseMetadata metadata;
    FILE *dbConnection;
} Database;


void prependNode(Node *currentNode, Node *newNode);

void removeNode(Node *node);

void serializeNode(Database *db, Node *node);

void serializeDatabase(Database *db);

void serializeTable(Database *db, Table *table);

void serializeColumn(Database *db, Column *column);

void serializeRow(Database *db, Row *row);

void serializeData(Database *db, Data *data);

Node* deserializeNode(Database *db, long offset);

Database* deserializeDatabase(Database *db, long offset);

Table* deserializeTable(Database *db, long offset);

Column* deserializeColumn(Database *db, long offset);

Row* deserializeRow(Database *db, long offset);

Data* deserializeData(Database *db, long offset);

#endif