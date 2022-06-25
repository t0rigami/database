#ifndef HDB_TYPES_H
#define HDB_TYPES_H

#include <cstring>
#include <iostream>

enum ColumnTypeEnum { INT, CHAR, VARCHAR };

typedef char byte;

struct byte_array {
    byte* value;
    size_t size;
};

#endif