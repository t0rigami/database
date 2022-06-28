#ifndef HDB_TUPLE_DATA_H
#define HDB_TUPLE_DATA_H

#include <vector>
#include "types.h"
#include "Column.h"
#include "Serializable.h"

struct TupleData;
typedef TupleData *TupleDataPtr;

struct TupleData : public Serializable {
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
    explicit TupleData(int tupleSize);

    TupleData();

    byte_array serialize() const override;

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

    void retireSpaceAndData() const;

    void printFormat(ColumnPtr *columns, int columnSize) const;

    void printFormat(const std::vector<ColumnPtr> &columns) const;

    void retireSpace() const;

    static void listSave(const std::vector<TupleData> &tuples, const std::string &tempPath);

    static TupleData fromBytes(byte_array bytes);
};

#endif