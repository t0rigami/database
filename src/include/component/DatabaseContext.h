#ifndef HDB_DATABASE_CONTEXT_H
#define HDB_DATABASE_CONTEXT_H

#include <iostream>
#include <unordered_map>
#include "DatabaseConfig.h"
#include "ServerSocketManager.h"
#include "Table.h"
#include "SqlExecutor.h"

#define HDB_LOG_TRACE

class DatabaseContext {
public:
    /**
     * @brief 获取配置文件
     *
     * @return const DatabaseConfig&
     */
    const DatabaseConfig &getConfig() const;

    /**
     * @brief 初始化配置
     *
     * @param path 配置文件目录
     */
    void initConfiguration(std::string path);

    /**
     * @brief 初始化 DB 的网络通信
     *
     */
    void initServerSocket();

    /**
     * 初始化系统表信息
     */
    void initTable();

    /**
     * 初始化 SQL 执行器
     */
    void initSqlExecutor();

    /**
     * @brief 开启服务
     *
     */
    int start();

    /**
     * 注册一个表
     * @param table 表
     * @param saveAction 是否立刻更新表的结构到 pg_class 和 pg_attribute
     * @return 是否成功
     */
    bool registerTable(TablePtr table, bool saveAction = false);

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

    /**
     * 获取所有的表映射
     * @return 表映射
     */
    std::unordered_map<int, TablePtr> &getTables();

    /**
     * 获取 SqlExecutor
     * @return  SqlExecutor
     */
    SqlExecutor &getSqlExecutor();

private:
    /**
     * 数据库相关配置
     */
    DatabaseConfig config;
    /**
     * 数据库通信管理
     */
    ServerSocketManager serverSocketManager;
    /**
     * 数据库表管理
     */
    std::unordered_map<int, TablePtr> tables;
    /**
     * Sql
     */
    SqlExecutor sqlExecutor;

    /**
     * 初始化用户定义的表
     * @param pgClassTable pg_class 表
     * @param pgAttributeTable pg_attribute 表
     */
    void initUserTable(TablePtr pgClassTable, TablePtr pgAttributeTable);
};

#endif  // HDB_DATABASE_CONTEXT_H