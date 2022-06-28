//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_TABLE_MANAGER_H
#define DB_SERVER_TABLE_MANAGER_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "Table.h"
#include "SqlExecutor.h"

class TableManager {
public:
    TableManager(SqlExecutor &sqlExecutor, std::string pageBasePath);

    ~TableManager();

    /**
     * 初始化系统表信息
     */
    void initTable();

    /**
     * 初始化用户定义的表
     */
    void initUserTable();

    /**
     * 注册一个表
     * @param table 表
     * @param insertClsAndAttr 是否需要将改表信息插入到 pg_class 和 pg_attribute
     * @param saveAction 是否立刻更新表的结构到 pg_class 和 pg_attribute
     * @return 是否成功
     */
    bool registerTable(TablePtr table, bool insertClsAndAttr = true, bool saveAction = false);

    /**
     * 通过表 id 获取表信息
     * @param tableId 表 id
     * @return 表 id 对应的表信息
     */
    TablePtr getTableById(int tableId);

    /**
     * 通过表的名称获取表信息
     * @param name 表名称
     * @return 表信息
     */
    TablePtr getTableByName(const std::string &name);

    std::unordered_map<int, TablePtr> &getTables();

    /**
     * 判断表是否存在
     * @param table
     * @return
     */
    bool exists(TablePtr table);

private:
    std::unordered_map<int, TablePtr> tables;
    SqlExecutor &sqlExecutor;
    std::string pageBasePath;
    // ==== pg_class ====
    // id           int
    // name         varchar
    // attr_count   int
    TablePtr pgClass;
    // ==== pg_attribute ====
    // table_id     int
    // name         varchar
    // type         int
    // _size        int
    // order        int
    // nullable     int
    TablePtr pgAttribute;

    /**
     * 将表插入到 pg_class 中
     * @param table 表信息
     */
    void insertClass(TablePtr table);

    /**
     * 将表插入到 pg_attribute 中
     * @param table 表信息
     */
    void insertAttribute(TablePtr table);

    /**
     * 创建 pg_class 表
     * @param pagePath 页面路径
     * @return pg_class 表
     */
    static TablePtr buildClassTable();

    /**
     * 创建 pg_attribute 表
     * @param pagePath 页面路径
     * @return pg_attribute 表
     */
    static TablePtr buildAttributeTable();

};

#endif //DB_SERVER_TABLE_MANAGER_H
