//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_SQL_EXECUTOR_H
#define DB_SERVER_SQL_EXECUTOR_H

#include "ResultSet.h"

class DatabaseContext;

class SqlExecutor {
public:
    void setContext(DatabaseContext *context);

    /**
     * 简单 select 查询
     * @param tableName 表名
     * @param logicalExpression 简单逻辑表达式
     * @param columns 选择的列
     * @return 结果集
     */
    ResultSet execSimpleQuery(const std::string &tableName, const std::string &logicalExpression,
                              std::initializer_list<ColumnPtr> columns);

private:
    DatabaseContext *context;
};

#endif //DB_SERVER_SQL_EXECUTOR_H
