#include "DatabaseContext.h"
#include "FileUtils.h"

void DatabaseContext::initConfiguration(std::string path) {
#ifdef HDB_LOG_TRACE
    printf("start init Configuration ...\n");
    printf("loading from path: %s\n", path.c_str());
#endif

    config = DatabaseConfig(path);

#ifdef HDB_LOG_TRACE
    printf("configuration init success\n");
    printf("port: %d\n", config.getPort());
#endif
}

const DatabaseConfig &DatabaseContext::getDatabaseConfig() const {
    return config;
}

void DatabaseContext::initServerSocket() {
#ifdef HDB_LOG_TRACE
    printf("start init ServerSocketManager ...\n");
#endif

    serverSocketManager = ServerSocketManager(config);

#ifdef HDB_LOG_TRACE
    printf("ServerSocketManager init success\n");
#endif
}

int DatabaseContext::start() {
    return serverSocketManager.start();
}

void DatabaseContext::initTable() {
    // 初始化表
    tableManager->initTable();
}

bool DatabaseContext::registerTable(TablePtr table, bool saveAction) {
    return tableManager->registerTable(table, saveAction);
}

void DatabaseContext::initSqlExecutor() {
    this->sqlExecutor.setTables(&tableManager->getTables());
}

SqlExecutor &DatabaseContext::getSqlExecutor() {
    return sqlExecutor;
}

TablePtr DatabaseContext::getTableById(int tableId) {
    return tableManager->getTableById(tableId);
}

TablePtr DatabaseContext::getTableByName(const std::string &name) {
    return tableManager->getTableByName(name);
}

TableManager *DatabaseContext::getTableManager() {
    return tableManager;
}

const ServerSocketManager &DatabaseContext::getServerSocketManager() const {
    return serverSocketManager;
}

DatabaseContext::~DatabaseContext() {
    delete tableManager;
}

DatabaseContext::DatabaseContext() {
    tableManager = new TableManager(sqlExecutor, config.getPagePath());
}
