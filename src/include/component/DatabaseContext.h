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

    bool registerTable(TablePtr table);

    const std::unordered_map<int32_t, TablePtr> &getTables() const;

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
    std::unordered_map<int32_t, TablePtr> tables;
    /**
     * Sql
     */
    SqlExecutor sqlExecutor;
};

#endif  // HDB_DATABASE_CONTEXT_H