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
    auto &tablePath = config.getTablePath();
    auto &pagePath = config.getPagePath();

    const std::string &pgClassPagePath = pagePath + "/" + std::to_string(Table::PG_CLASS_ID) + ".pg";
    const std::string &pgAttributePagePath = pagePath + "/" + std::to_string(Table::PG_ATTRIBUTE_ID) + ".pg";

//    const std::string &pgClassTablePath = tablePath + "/pg_class.tb";
//    const std::string &pgAttributeTablePath = tablePath + "/pg_attribute.tb";

    TablePtr pgClassTable = nullptr;
    TablePtr pgAttributeTable = nullptr;

    if (!FileUtils::exists(pgClassPagePath)) {
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
/*        for (int i = 0; i < pgClassTable->getPage(0)->getCount(); i++) {
            pgClassTable->getPage(0)->getTuples()[i].printFormat(pgClassTable->getColumns(),
                                                                 pgClassTable->getColumnSize());
        }
        for (int i = 0; i < pgAttributeTable->getPage(0)->getCount(); i++) {
            pgAttributeTable->getPage(0)->getTuples()[i].printFormat(pgAttributeTable->getColumns(),
                                                                     pgAttributeTable->getColumnSize());
        }*/
    }

    // 插入映射
    this->tables.insert({Table::PG_ATTRIBUTE_ID, pgAttributeTable});
    this->tables.insert({Table::PG_CLASS_ID, pgClassTable});
    // 保存
    pgClassTable->saveAllPage();
    pgAttributeTable->saveAllPage();
}

bool DatabaseContext::registerTable(TablePtr table) {
    if (this->tables.count(table->getTableId())) return false;

    // 生成表的页面路径
    table->setPagePath(config.getPagePath() + "/" + std::to_string(table->getTableId()) + ".pg");

    // 插入到表的 Map
    this->tables.insert({table->getTableId(), table});
    // 插入表信息
    Table::insertTable(tables[Table::PG_CLASS_ID], table);
    // 插入表列信息
    Table::insertAttrs(tables[Table::PG_ATTRIBUTE_ID], table);
    return true;
}

const std::unordered_map<int32_t, TablePtr> &DatabaseContext::getTables() const {
    return tables;
}

void DatabaseContext::initSqlExecutor() {
    this->sqlExecutor.setContext(this);
}
