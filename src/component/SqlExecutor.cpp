//
// Created by Origami on 2022/6/27.
//
#include "SqlExecutor.h"

ResultSet SqlExecutor::execSimpleQuery(const std::string &tableName, const std::string &logicalExpression,
                                       std::initializer_list<ColumnPtr> columns) {
    return ResultSet();
}

void SqlExecutor::setContext(DatabaseContext *ctx) {
    this->context = ctx;
}
