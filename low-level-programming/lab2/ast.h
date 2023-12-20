#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef struct Column {
    char* name;
    struct Column* next; // For a linked list of columns
} Column;

typedef struct CreateTable {
    char* tableName;
    Column* columns;
} CreateTable;

#endif
