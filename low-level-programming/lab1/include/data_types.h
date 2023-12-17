#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Define basic data types for the database
typedef int64_t db_int;    // Integer type
typedef double db_float;   // Floating-point type
typedef char* db_string;   // String type
typedef bool db_bool;      // Boolean type

// Enumeration for identifying the type of data in a Data structure
typedef enum {
    DT_INT,     // Represents an integer
    DT_FLOAT,   // Represents a floating-point number
    DT_STRING,  // Represents a string
    DT_BOOL     // Represents a boolean
} DataType;

// Structure to store dynamically typed data
typedef struct {
    DataType type;  // Type of the data
    union {
        db_int int_val;     // Store integer if type is DT_INT
        db_float float_val; // Store float if type is DT_FLOAT
        db_string string_val; // Store string if type is DT_STRING
        db_bool bool_val;   // Store boolean if type is DT_BOOL
    };
} Data;

#endif // DATA_TYPES_H
