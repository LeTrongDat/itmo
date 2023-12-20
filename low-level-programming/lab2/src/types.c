#include "types.h"
#include <stdio.h>

void printAST(CreateTable* createTable) {
    if (createTable == NULL) return;

    printf("Create Table: %s\n", createTable->tableName);
    printf("Columns:\n");

    Column* col = createTable->columns;
    while (col) {
        printf(" - %s\n", col->name);
        col = col->next;
    }
}
