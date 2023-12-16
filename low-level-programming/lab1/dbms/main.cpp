// main.cpp
#include "database.h"
#include <iostream>

int main() {
    Database db;
    db.createTable("Students", {{"ID", DataType::INT32}, {"Name", DataType::STRING}});
    
    Row newRow;
    newRow.data["ID"] = "1";
    newRow.data["Name"] = "John Doe";
    db.insertRow("Students", newRow);

    // Additional code to interact with the database...
    return 0;
}
