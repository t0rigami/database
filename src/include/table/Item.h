#ifndef HDB_ITEM_H
#define HDB_ITEM_H

#include <iostream>

#define _ITEM_SIZE

struct Item;
typedef Item *ItemPtr;

struct Item {
    /**
     * @brief 数据项所在的偏移量
     *
     */
    size_t offset;
    /**
     * @brief 数据项的大小
     *
     */
    size_t size;

    const static size_t ITEM_SIZE = sizeof(size_t) * 2;
};

#endif