//
// Created by Origami on 2022/6/27.
//
#include "TableManager.h"

#include <utility>
#include "FileUtils.h"

void TableManager::initTable() {
    // 先构建系统表
    pgClass = buildClassTable();
    pgAttribute = buildAttributeTable();

    // 注册到系统中，此时先不将两张表自己注册自己
    registerTable(pgClass, false);
    registerTable(pgAttribute, false);

    // 判断文件中系统表的记录是否存在
    bool exists = FileUtils::exists(pgClass->getPagePath()) &&
                  FileUtils::exists(pgAttribute->getPagePath());


    // 如果不存在则直接将系统表保存到文件中，然后返回
    if (!exists) {
        // 不存在的话就说明没有，此时需要让两张表自己插入自己的信息
        insertClass(pgClass);
        insertAttribute(pgClass);
        insertClass(pgAttribute);
        insertAttribute(pgAttribute);

        pgClass->saveAllPage();
        pgAttribute->saveAllPage();
        return;
    }

    // 存在则表明之前创建过，则将非系统表读取加载到内存
    initUserTable();
}

void TableManager::initUserTable() {
    // 从 pg_class 中读取表信息
    ResultSet pgClassResultSet =
            // select * from pg_class where id != 1259;
            sqlExecutor.execSimpleQuery("pg_class", "id != 1259", {});

    // 遍历查询结果集
    for (const auto &classTuple: pgClassResultSet.getRawResult()) {
        // 选择出表名
        std::string name = pgClass->selectString("name", classTuple);
        // 选择出表 id
        int tableId = pgClass->selectInt("id", classTuple);

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
                sqlExecutor.execSimpleQuery(
                        "pg_attribute", "table_id = " + std::to_string(tableId), {});

        // 遍历列信息
        for (const auto &attrTuple: pgAttributeResultSet.getRawResult()) {
            // 获取列名
            std::string colName = pgAttribute->selectString("name", attrTuple);
            // 获取列的类型
            ColumnTypeEnum type =
                    (ColumnTypeEnum) pgAttribute->selectInt("type", attrTuple);
            // 获取列的大小
            int size = pgAttribute->selectInt("size", attrTuple);
            // 获取当前列是否为空
            bool nullable = pgAttribute->selectInt("nullable", attrTuple);
            // 加入这个列
            builder.addColumn(colName, type, size, nullable);
        }

        // 注册这个表
        registerTable(builder.build());
    }
}

bool TableManager::registerTable(TablePtr table, bool insertClsAndAttr, bool saveAction) {
    if (exists(table)) {
        return false;
    }

    for (const auto &item: tables) item.second->summary();

    // 生成表的页面路径
    table->setPagePath(pageBasePath + "/" + std::to_string(table->getTableId()) +
                       ".pg");

    // 插入到表的 Map
    this->tables.insert({table->getTableId(), table});

    if (insertClsAndAttr) {
        // 插入表信息
        insertClass(table);
        // 插入表列信息
        insertAttribute(table);
    }
    if (saveAction) {
        pgClass->saveAllPage();
        pgAttribute->saveAllPage();
    }

    return true;
}

TablePtr TableManager::getTableById(int tableId) {
    if (tables.count(tableId)) return tables[tableId];
    return nullptr;
}

TablePtr TableManager::getTableByName(const std::string &name) {
    for (const auto &item: tables) {
        if (item.second->getName() == name) return item.second;
    }
    return nullptr;
}

std::unordered_map<int, TablePtr> &TableManager::getTables() {
    return tables;
}

TableManager::TableManager(SqlExecutor &sqlExecutor, std::string pageBasePath)
        : sqlExecutor(sqlExecutor),
          pageBasePath(std::move(pageBasePath)),
          pgClass(nullptr),
          pgAttribute(nullptr) {
}

void TableManager::insertClass(TablePtr table) {
//    printf("Insert Class name=%s, id=%d\n", table->getName().c_str(), table->getTableId());
    if (pgClass->getPages().empty()) pgClass->loadAllPage();
    TupleDataPtr tuple = TupleData::Builder()
            .addInt(table->getTableId())
            .addInt(table->getColumnSize())
            .addString(table->getName())
            .build();
    pgClass->insert(*tuple);
}

void TableManager::insertAttribute(TablePtr table) {
    if (pgAttribute->getPages().empty()) pgAttribute->loadAllPage();
    for (int i = 0; i < table->getColumnSize(); i++) {
        ColumnPtr column = table->getColumns()[i];
        TupleDataPtr tuple = TupleData::Builder()
                .addInt(table->getTableId())
                .addInt(column->type)
                .addInt(column->size)
                .addInt(column->order)
                .addInt(column->nullable)
                .addString(column->name)
                .build();
        // 插入
        pgAttribute->insert(*tuple);
    }
}

TablePtr TableManager::buildClassTable() {
    return Table::Builder()
            .tableId(Table::PG_CLASS_ID)
            .name("pg_class")
            .addColumn("id", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("attr_count", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("name", ColumnTypeEnum::VARCHAR, 50)
            .build();
}

TablePtr TableManager::buildAttributeTable() {
    return Table::Builder()
            .tableId(Table::PG_ATTRIBUTE_ID)
            .name("pg_attribute")
            .addColumn("table_id", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("type", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("size", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("order", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("nullable", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("name", ColumnTypeEnum::VARCHAR, 50)
            .build();
}

TableManager::~TableManager() {
    for (const auto &item: this->tables) item.second->saveAllPage();
}

bool TableManager::exists(TablePtr table) {
    if (tables.count(table->getTableId())) return true;
    for (const auto &item: tables)
        if (item.second->getName() == table->getName())
            return true;
    return false;
}
