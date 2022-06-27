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

const DatabaseConfig &DatabaseContext::getConfig() const {
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
    // 从文件中加载系统表信息
    auto &pagePath = config.getPagePath();

    const std::string &pgClassPagePath = pagePath + "/" + std::to_string(Table::PG_CLASS_ID) + ".pg";
    const std::string &pgAttributePagePath = pagePath + "/" + std::to_string(Table::PG_ATTRIBUTE_ID) + ".pg";

    // 判断系统表是否存在
    bool exists = FileUtils::exists(pgClassPagePath) && FileUtils::exists(pgAttributePagePath);

    TablePtr pgClassTable = nullptr;
    TablePtr pgAttributeTable = nullptr;

    if (!exists) {
        // 不存在，就新建
        pgClassTable = Table::buildClassTable(pgClassPagePath);

        pgAttributeTable = Table::buildAttributeTable(pgAttributePagePath);

        // 将 pg_class 和 pg_attribute 表信息插入到 pg_class 中

        // 将两张表的信息插入到 pg_class 表中
        Table::insertTable(pgClassTable, pgClassTable);
        Table::insertTable(pgClassTable, pgAttributeTable);

        // 将两张表的列属性插入到 pg_attribute 表中
        Table::insertAttrs(pgAttributeTable, pgClassTable);
        Table::insertAttrs(pgAttributeTable, pgAttributeTable);

    } else {
        // 如果存在就加载
        pgClassTable = Table::loadClass(pgClassPagePath);
        pgAttributeTable = Table::loadAttribute(pgAttributePagePath);
    }

    // 插入映射
    this->tables.insert({Table::PG_ATTRIBUTE_ID, pgAttributeTable});
    this->tables.insert({Table::PG_CLASS_ID, pgClassTable});

    if (!exists) {
        // 如果一开始不存在，则保存
        pgClassTable->saveAllPage();
        pgAttributeTable->saveAllPage();
        return;
    }

    // 将非系统表生成，插入到 tables 中
    initUserTable(pgClassTable, pgAttributeTable);
}

bool DatabaseContext::registerTable(TablePtr table, bool saveAction) {
    if (this->tables.count(table->getTableId())) return false;

    // 生成表的页面路径
    table->setPagePath(config.getPagePath() + "/" + std::to_string(table->getTableId()) + ".pg");

    // 插入到表的 Map
    this->tables.insert({table->getTableId(), table});

    TablePtr pgClass = tables[Table::PG_CLASS_ID];
    TablePtr pgAttribute = tables[Table::PG_ATTRIBUTE_ID];

    // 插入表信息
    Table::insertTable(pgClass, table);
    // 插入表列信息
    Table::insertAttrs(pgAttribute, table);

    if (saveAction) {
        pgClass->saveAllPage();
        pgAttribute->saveAllPage();
    }

    return true;
}

std::unordered_map<int, TablePtr> &DatabaseContext::getTables() {
    return tables;
}

void DatabaseContext::initSqlExecutor() {
    this->sqlExecutor.setTables(&tables);
}

SqlExecutor &DatabaseContext::getSqlExecutor() {
    return sqlExecutor;
}

void DatabaseContext::initUserTable(TablePtr pgClassTable, TablePtr pgAttributeTable) {
    // 从 pg_class 中读取表信息
    ResultSet pgClassResultSet =
            // select * from pg_class where id != 1259;
            sqlExecutor.execSimpleQuery("pg_class", "id != 1259", {});

    // 遍历结果集
    for (const auto &classTuple: pgClassResultSet.getRawResult()) {

        // 选择出表名
        std::string name = pgClassTable->selectString("name", classTuple);
        // 选择出表 id
        int tableId = pgClassTable->selectInt("id", classTuple);

        // 需要过滤掉当前已经存在的 pg_attribute 表
        if (tableId == Table::PG_ATTRIBUTE_ID) {
            continue;
        }
        // 构建一个表对象
        Table::Builder builder;

        builder.name(name).tableId(tableId);

        // 从 pg_attribute 中读取表 ID 为 tableId 的列信息
        ResultSet pgAttributeResultSet =
                // select * from pg_attribute where table_id = ${tableId}
                sqlExecutor.execSimpleQuery("pg_attribute", "table_id = " + std::to_string(tableId), {});

        // 遍历列信息
        for (const auto &attrTuple: pgAttributeResultSet.getRawResult()) {
            // 获取列名
            std::string colName = pgAttributeTable->selectString("name", attrTuple);
            // 获取列的类型
            ColumnTypeEnum type = (ColumnTypeEnum) pgAttributeTable->selectInt("type", attrTuple);
            // 获取列的大小
            int size = pgAttributeTable->selectInt("size", attrTuple);
            // 获取当前列是否为空
            bool nullable = pgAttributeTable->selectInt("nullable", attrTuple);
            // 加入这个列
            builder.addColumn(colName, type, size, nullable);
        }

        // 注册这个表
        registerTable(builder.build());
    }
}

TablePtr DatabaseContext::getTableById(int tableId) {
    if (tables.count(tableId)) return tables[tableId];
    return nullptr;
}

TablePtr DatabaseContext::getTableByName(const std::string &name) {
    for (const auto &item: tables) {
        if (item.second->getName() == name)
            return item.second;
    }
    return nullptr;
}
