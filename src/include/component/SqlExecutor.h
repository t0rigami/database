//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_SQL_EXECUTOR_H
#define DB_SERVER_SQL_EXECUTOR_H

#include "ResultSet.h"
#include <unordered_map>
#include <Table.h>
#include "SqlParser.h"

class SqlExecutor {
public:
    SqlExecutor();

    /**
     * 简单 select 查询
     * @param tableName 表名
     * @param logicalExpression 简单逻辑表达式
     * @param columns 选择的列
     * @return 结果集
     */
    ResultSet execSimpleQuery(const std::string &tableName, const std::string &logicalExpression,
                              std::initializer_list<std::string> selectedName);

    /**
     * 设置 _tables
     * @param _tables 表
     */
    void setTables(std::unordered_map<int, TablePtr> *_tables);

    static bool eval(ColumnPtr column, const TupleData &tuple, BiOperator op, int rightOperand);

private:
    std::unordered_map<int, TablePtr> *tables;
};

#endif //DB_SERVER_SQL_EXECUTOR_H
