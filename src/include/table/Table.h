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
                           int columnSize,
                           bool nullable = true);

        /**
         * 设置存储页面的路径
         * @param pagePath 页面路径
         * @return Builder&
         */
        Builder &pagePath(const std::string &pagePath);

        /**
         * 设置表的 id
         * @param tableId 表 id
         * @return Builder&
         */
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
    void saveOnePage(int pageNum);

    /**
     * @brief 保存一个范围内的所有页
     *
     */
    void saveRangePage(int lRange, int rRange);

    /**
     * 保存所有页面
     */
    void saveAllPage();

    /**
     * 读取某一个记录
     * @param pageNum 页码
     * @param count 项目
     * @return Tuple
     */
    TupleData getRecordFromFile(int pageNum, int count);

    /**
     * 获取页面总数
     * @return
     */
    int getPageSize() const;

    /**
     * 格式化打印一个 Tuple
     * @param tuple
     */
    void formatPrint(const TupleData &tuple);

    /**
     * 获取所有页面
     * @return 页面列表
     */
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
    PagePtr getPage(int pageNum);

    /**
     * 从表结构文件中加载一张表
     * @param tablePath 表结构路径
     */
    explicit Table(const std::string &tablePath);

    /**
     * 获取表 Id
     * @return 表 Id
     */
    int32_t getTableId() const;

    /**
 * 设置表存储路径
 * @param path
 */
    void setPagePath(const std::string &path);

    /**
     * 获取所有表列信息
     * @return
     */
    ColumnPtr *getColumns() const;

    /**
     * 获取表列的个数
     * @return
     */
    int getColumnSize() const;

    /**
     * 根据列名选择除列信息
     * @param columnName 列名
     * @return 列信息
     */
    ColumnPtr selectColumn(const std::string &columnName);

    /**
     * 加载一个页面
     * @param pageNum 页码
     */
    void loadPage(int pageNum);

    /**
     * 加载一个范围内的页面 [lRange, rRange]
     * @param lRange 左边界
     * @param rRange 有边界
     */
    void loadPage(int lRange, int rRange);

    /**
     * 加载所有页面
     */
    void loadAllPage();

    const std::string &getPagePath() const;

#pragma region Public Static Property Definition

    /**
     * pg_class 系统表 ID 1259
     */
    const static int PG_CLASS_ID;
    /**
     * pg_attribute 系统 ID 1249
     */
    const static int PG_ATTRIBUTE_ID;

    /**
     * 创建一张表
     * @param tableName 表名称
     * @param columns 表的列信息
     * @return 表
     */
    static TablePtr create(const std::string &tableName, int tableId, std::initializer_list<Column> columns);

#pragma endregion

    std::string selectString(const std::string &columnName, const TupleData &tuple);

    int selectInt(const std::string &columnName, const TupleData &tuple);

private:
    /**
     * 默认空构造
     */
    Table();

    /**
     * 表 id
     */
    int tableId;
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
    ColumnPtr *columns;
    /**
     * 列的个数
     */
    int size;
    /**
     * 表存储的路径
     */
    std::string tablePath;
    /**
     * 表中页存储的路径
     */
    std::string pagePath;
};

#endif