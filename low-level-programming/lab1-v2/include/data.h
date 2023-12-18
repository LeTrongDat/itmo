#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Enumeration for data types
typedef enum {
    DT_INT,
    DT_FLOAT,
    DT_STRING,
    DT_BOOL
} DataType;

// Union to hold different types of data values
typedef union {
    int int_val;
    float float_val;
    char *string_val;
    bool bool_val;
} DataValue;

// Structure to manage data-related file offsets
typedef struct {
    long offset;          // Offset of this Data in the file
    long nextDataOffset;  // Offset to the next Data in the file
    long prevDataOffset;  // Offset to the previous Data in the file
} FileData;

// Data structure
typedef struct Data {
    DataType type;           // Data type
    size_t dataSize;         // Size of the data, important for variable-length data like strings
    DataValue value;         // Actual data value
    struct Data *prev;       // Pointer to the previous Data
    struct Data *next;       // Pointer to the next Data
    FileData fileData;       // File-related metadata
} Data;

// Serialization and deserialization function prototypes
void serializeData(FILE *file, const Data *data);
void deserializeData(FILE *file, Data *data);

// Helper function prototypes for navigating through data nodes
Data* getNextData(FILE *file, Data *currentData);
Data* getPrevData(FILE *file, Data *currentData);

// Function to free a Data object
void freeData(Data* data);

// Function to free all Data objects in a row
void freeDatas(Data* lastData);

#endif // DATA_H
