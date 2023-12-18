#include "../include/data.h"
#include <string.h>

// Serialization function for a Data object
void serializeData(FILE *file, const Data *data) {
    if (!file || !data) return;

    if (data->fileData.offset) fseek(file, data->fileData.offset, SEEK_SET);

    // Write the data type
    fwrite(&(data->type), sizeof(data->type), 1, file);

    // Write the dataSize
    fwrite(&(data->dataSize), sizeof(data->dataSize), 1, file);

    // Write the data based on its type
    switch (data->type) {
        case DT_INT:
            fwrite(&(data->value.int_val), sizeof(data->value.int_val), 1, file);
            break;
        case DT_FLOAT:
            fwrite(&(data->value.float_val), sizeof(data->value.float_val), 1, file);
            break;
        case DT_STRING:
            // Write the string itself
            fwrite(data->value.string_val, sizeof(char), data->dataSize, file);
            break;
        case DT_BOOL:
            fwrite(&(data->value.bool_val), sizeof(data->value.bool_val), 1, file);
            break;
    }

    // Write the FileData (offsets)
    fwrite(&(data->fileData), sizeof(data->fileData), 1, file);
}

// Deserialization function for a Data object
void deserializeData(FILE *file, Data *data) {
    if (!file || !data) return;

    if (data->fileData.offset) fseek(file, data->fileData.offset, SEEK_SET);

    // Read the data type
    fread(&(data->type), sizeof(data->type), 1, file);

    // Read the dataSize
    fread(&(data->dataSize), sizeof(data->dataSize), 1, file);

    // Read the data based on its type
    switch (data->type) {
        case DT_INT:
            fread(&(data->value.int_val), sizeof(data->value.int_val), 1, file);
            break;
        case DT_FLOAT:
            fread(&(data->value.float_val), sizeof(data->value.float_val), 1, file);
            break;
        case DT_STRING:
            // Allocate memory for the string and read it
            data->value.string_val = malloc(data->dataSize * sizeof(char));
            if (data->value.string_val != NULL) {
                fread(data->value.string_val, sizeof(char), data->dataSize, file);
            }
            break;
        case DT_BOOL:
            fread(&(data->value.bool_val), sizeof(data->value.bool_val), 1, file);
            break;
    }

    // Read the FileData (offsets)
    fread(&(data->fileData), sizeof(data->fileData), 1, file);
}

// Function to get the next Data node
Data* getNextData(FILE *file, Data *currentData) {
    if (!currentData) return NULL;
    if (currentData->next) return currentData->next;

    // Load from file if the next node is not in memory and file is provided
    if (file && currentData->fileData.nextDataOffset != 0) {
        Data* nextData = malloc(sizeof(Data));
        if (!nextData) return NULL;

        fseek(file, currentData->fileData.nextDataOffset, SEEK_SET);
        deserializeData(file, nextData);
        currentData->next = nextData;
        return nextData;
    }

    return NULL;
}

// Function to get the previous Data node, renamed for consistency
Data* getPrevData(FILE *file, Data *currentData) {
    if (!currentData) return NULL;
    if (currentData->prev) return currentData->prev;

    // Load from file if the previous node is not in memory and file is provided
    if (file && currentData->fileData.prevDataOffset != 0) {
        Data* prevData = malloc(sizeof(Data));
        if (!prevData) return NULL;

        fseek(file, currentData->fileData.prevDataOffset, SEEK_SET);
        deserializeData(file, prevData);
        currentData->prev = prevData;
        return prevData;
    }

    return NULL;
}

// Function to free an individual Data object
void freeData(Data* data) {
    if (!data) return;

    // Free any dynamically allocated data within the Data object
    if (data->type == DT_STRING && data->value.string_val) {
        free(data->value.string_val);
    }

    // Finally, free the Data object itself
    free(data);
}

// Function to free all Data objects in a row starting from the last one
void freeDatas(Data* lastData) {
    while (lastData) {
        Data* temp = lastData;
        lastData = lastData->prev;  // Move to the previous Data object

        freeData(temp);  // Free the current Data object
    }
}
