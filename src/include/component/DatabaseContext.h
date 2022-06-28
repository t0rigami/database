#ifndef HDB_DATABASE_CONTEXT_H
#define HDB_DATABASE_CONTEXT_H

#include <iostream>
#include <unordered_map>
#include "DatabaseConfig.h"
#include "ServerSocketManager.h"
#include "Table.h"
#include "SqlExecutor.h"
#include "TableManager.h"

#define HDB_LOG_TRACE

class DatabaseContext {

public:
    DatabaseContext();

    ~DatabaseContext();

#pragma region Config

public:

    /**
     * @brief 初始化配置
     *
     * @param path 配置文件目录
     */
    void initConfiguration(std::string path);

    /**
    * @brief 获取配置文件
    *
    * @return const DatabaseConfig&
    */
    const DatabaseConfig &getDatabaseConfig() const;

private:
    /**
     * 数据库相关配置
     */
    DatabaseConfig config;

#pragma endregion

#pragma region Socket

public:

    /**
     * @brief 初始化 DB 的网络通信
     *
     */
    void initServerSocket();

    /**
     * @brief 开启服务
     *
     */
    int start();

    const ServerSocketManager &getServerSocketManager() const;

private:
    /**
     * 数据库通信管理
     */
    ServerSocketManager serverSocketManager;
#pragma endregion

#pragma region Table

public:

    /**
     * 初始化系统表信息
     */
    void initTable();

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

    TableManager *getTableManager();

private:
    /**
     * 数据库表管理
     */
    TableManager *tableManager;
#pragma endregion

#pragma region SqlExecutor

public:
    /**
     * 初始化 SQL 执行器
     */
    void initSqlExecutor();

    /**
     * 获取 SqlExecutor
     * @return  SqlExecutor
     */
    SqlExecutor &getSqlExecutor();

private:
    /**
     * Sql
     */
    SqlExecutor sqlExecutor{};
#pragma endregion
};

#endif  // HDB_DATABASE_CONTEXT_H