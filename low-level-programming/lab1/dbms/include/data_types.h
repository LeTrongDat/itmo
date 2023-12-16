// include/data_types.h
#pragma once
#include <cstdint>
#include <string>
#include <map>

enum DataType { INT32, FLOAT, STRING, BOOL };

struct Column {
    std::string name;
    DataType type;
};

struct Row {
    std::map<std::string, std::string> data;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
};
