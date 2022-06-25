#ifndef HDB_TABLE
#define HDB_TABLE

#include <unordered_map>
#include <vector>
#include "Column.h"
#include "MallocUtils.h"
#include "Page.h"
#include "Serializable.h"
#include "TupleData.h"
#include <initializer_list>

class Table;

typedef Table *TablePtr;

/**
 * @brief 表对象
 *
 */
class Table : public Serializable {
public:
#pragma region Table::Builder definition

    /**
     * @brief 创建表对象的 Builder
     *
     */
    class Builder {
    public:
        Builder();

        /**
         * @brief 设置表名
         *
         * @param name 表名
         * @return Builder&
         */
        Builder &name(const std::string &name);

        /**
         * @brief 添加列
         *
         * @param columnName 列名
         * @param type 列的类型
         * @param columnSize 列的大小
         * @return Builder&
         */
        Builder &addColumn(const std::string &columnName,
                           ColumnTypeEnum type,
                           int columnSize);

        Builder &pagePath(const std::string &pagePath);

        Builder &tableId(int32_t tableId);

        /**
         * @brief 得到 Table 对象
         *
         * @return TablePtr
         */
        TablePtr build();

    private:
        std::vector<Column *> columns;
        TablePtr product;
        int32_t id{};
    };

#pragma endregion

    /**
     * @brief 索引获取列信息
     *
     * @param columnName 列的名称
     * @return ColumnPtr
     */
    ColumnPtr operator[](const std::string &columnName);

    /**
     * @brief 索引获取列信息
     *
     * @param columnName 列的名称
     * @return ColumnPtr
     */
    ColumnPtr operator[](const char *columnName);

    /**
     * @brief 获取表名
     *
     * @return std::string
     */
    std::string getName();

    /**
     * @brief 获取所有的列名
     *
     * @return std::vector<char*>
     */
    std::vector<char *> getColumnNames();

    /**
     * @brief 打印表的总信息
     *
     */
    void summary();

    /**
     * @brief 序列化
     *
     * @return byte_array
     */
    byte_array serialize() const override;

    /**
     * @brief 保存到文件
     *
     * @param path
     */
    void saveTableStruct(const std::string &path);

    /**
     * @brief 保存某个位置的 page 对象
     *
     * @param pageNum
     */
    void saveOnePage(size_t pageNum);

    /**
     * @brief 保存一个范围内的所有页
     *
     */
    void saveRangePage(size_t lRange, size_t rRange);

    void saveAllPage();

    TupleData getRecordFromFile(size_t pageNum, size_t count);

    int getPageSize() const;

    void print(const TupleData &tuple);

    std::vector<PagePtr> &getPages();

    /**
     * @brief 插入一条数据
     *
     * @param tupleData 元组
     * @return size_t 受影响行数
     */
    size_t insert(const TupleData &tupleData);

    /**
     * @brief 获取页面
     *
     * @param pageNum
     * @return PagePtr
     */
    PagePtr getPage(size_t pageNum);

    explicit Table(const std::string &tablePath);

    int32_t getTableId() const;

    const static int PG_CLASS_ID;
    const static int PG_ATTRIBUTE_ID;

    /**
     * 插入表的属性到属性表
     * @param attrTable 属性表
     * @param table 目标表
     */
    static void insertAttrs(TablePtr attrTable, TablePtr table);

    /**
     * 插入表到表信息表
     * @param classTable 表信息表
     * @param table 目标表
     */
    static void insertTable(TablePtr classTable, TablePtr table);

    static TablePtr loadClass(const std::string &classPath);

    static TablePtr loadAttribute(const std::string &attributePath);

    static TablePtr buildClassTable(const std::string &pagePath);

    static TablePtr buildAttributeTable(const std::string &pagePath);

    static TablePtr create(const std::string &tableName, std::initializer_list<Column> columns);

    void setPagePath(const std::string &path);

    ColumnPtr *getColumns() const;

    int getColumnSize() const;

private:
    static int TABLE_ID_COUNT;

    Table();

    /**
     * 表 id
     */
    int32_t tableId{};
    /**
     * 表名
     */
    std::string name;
    /**
     * 表的每一列
     */
    std::unordered_map<std::string, ColumnPtr> columnMap;
    /**
     * 内存中的页面
     */
    std::vector<PagePtr> pages;
    /**
     * 列
     */
    ColumnPtr *columns{};
    /**
     * 列的个数
     */
    int size{};
    /**
     * 表存储的路径
     */
    std::string tablePath;
    /**
     * 表中页存储的路径
     */
    std::string pagePath;

    void loadPage(int pageNum);

    void loadPage(int lRange, int rRange);
};

#endif