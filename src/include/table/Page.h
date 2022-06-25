#ifndef HDB_PAGE_H
#define HDB_PAGE_H

#include <vector>
#include "ByteWriter.h"
#include "Item.h"
#include "PageHeaderData.h"
#include "Serializable.h"
#include "TupleData.h"
#include "Column.h"

class Page;

typedef Page *PagePtr;

class Page : public Serializable {
public:
    explicit Page(int32_t pageState);

    explicit Page(const byte_array &bytes, ColumnPtr *columns, int size);

    /**
     * @brief 获取当前大小
     *
     * @return size_t
     */
    size_t getTotalSize() const;

    /**
     * @brief 判断是否还有足够的空间
     *
     * @param size 空间
     * @return true 有
     * @return false 没有
     */
    bool isRemain(size_t size) const;

    bool isRemain(TupleData size) const;

    /**
     * @brief 追加单元
     *
     * @param item
     * @param tupleData
     * @return 是否添加成功
     */
    bool append(TupleData tupleData);

    /**
     * @brief 移除索引位置的元素
     *
     * @param index 索引
     */
    void remove(size_t index);

    /**
     * @brief 移除索引位置的元素，同时释放它们的内存
     *
     * @param index 索引
     */
    void removeAndRetire(size_t index);

    bool hasStates(int32_t states) const;

    void writerTo(ByteWriter &bw);

    byte_array serialize() const override;

    const std::vector<TupleData> &getTuples() const;

    /**
     * @brief 页大小
     *
     */
    const static int PAGE_SIZE = 8192;
    /**
     * @brief special 大小
     *
     */
    const static int SPECIAL_SIZE = 200;

    // public field
    int getCount();

public:
    const static int32_t APPEND = 1 << 0;
    const static int32_t MODIFY = 1 << 1;
    /**
     * @brief 当前页的状态
     *
     */
    int32_t state{};

    PageHeaderData pageHeader{};
private:
    std::vector<TupleData> tuples;
};

#endif