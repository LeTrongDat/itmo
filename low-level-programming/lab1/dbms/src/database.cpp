// src/database.cpp
#include "database.h"

void Database::createTable(const std::string& name, const std::vector<Column>& columns) {
    Table newTable;
    newTable.name = name;
    newTable.columns = columns;
    tables[name] = newTable;
}

void Database::insertRow(const std::string& tableName, const Row& row) {
    tables[tableName].rows.push_back(row);
}

// Implement other functions here...
