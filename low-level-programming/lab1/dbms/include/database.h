// include/database.h
#pragma once
#include "data_types.h"
#include <map>

class Database {
private:
    std::map<std::string, Table> tables;

public:
    void createTable(const std::string& name, const std::vector<Column>& columns);
    void insertRow(const std::string& tableName, const Row& row);
    // Other functions like updateRow, deleteRow, selectRows...
};
