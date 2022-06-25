#ifndef HDB_TUPLE_DATA_H
#define HDB_TUPLE_DATA_H

#include <vector>
#include "types.h"
#include "Column.h"

struct TupleData;
typedef TupleData *TupleDataPtr;

struct TupleData {
    class Builder {
    public:
        Builder &addString(const std::string &str);

        Builder &addInt(int i);

        Builder &addChar(char ch);

        TupleDataPtr build();

    private:
        std::vector<std::string> strs;
        std::vector<int> ints;
        std::vector<char> chars;
        std::vector<ColumnTypeEnum> order;
    };

    /**
     * @brief 元组对应的数据项的指针
     *
     */
    void **data;
    /**
     * @brief 每个元组的数据大小
     *
     */
    size_t *dataSize;

    /**
     * @brief 元组的大小
     *
     */
    size_t length;

    size_t getTotalSize() const;

    void retire() const;

    void printFormat(ColumnPtr *columns, int columnSize) const;
};

#endif