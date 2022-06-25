#ifndef HDB_PAGE_HEADER_DATA_H
#define HDB_PAGE_HEADER_DATA_H

#include <iostream>

struct PageHeaderData;
typedef PageHeaderData *PageHeaderDataPtr;

struct PageHeaderData {
    const static int PAGE_HEADER_DATA_SIZE =
            sizeof(int64_t) + 4 * sizeof(int);

    int64_t tableId;
    int start;
    int lower;
    int upper;
    int special;
};

#endif