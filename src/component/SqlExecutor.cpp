//
// Created by Origami on 2022/6/27.
//
#include "SqlExecutor.h"
#include "Assert.h"
#include "StringUtils.h"

#define tableId first
#define table second

ResultSet SqlExecutor::execSimpleQuery(const std::string &tableName, const std::string &logicalExpression,
                                       std::initializer_list<std::string> selectedName) {
    TablePtr target = nullptr;
    // 遍历找到表名对应的表
    for (const auto &item: (*tables)) {
        if (item.table->getName() == tableName) {
            target = item.table;
            break;
        }
    }
    Assert::nonNull(target, "at SqlExecutor::execSimpleQuery: can not find " + tableName + " table");
    // 解析表达式
    Optional<BiRelation> bi = SqlParser::toBiRelation(logicalExpression);
    Assert::isFalse(bi.empty(), "at SqlExecutor::execSimpleQuery: expression error! " + logicalExpression);

    BiRelation rel = bi.get();

    bool left = StringUtils::isNumber(rel.getLeft());
    bool right = StringUtils::isNumber(rel.getRight());

    Assert::isFalse((left && right) || (!left && !right),
                    "at SqlExecutor::execSimpleQuery: expression error! " + logicalExpression);

    // 将表达式变为 左边是列，右边是操作数
    if (left && !right) rel.swapOperand();

    auto op = rel.getOp();
    auto &leftOperand = rel.getLeft();
    auto rightOperand = StringUtils::parseInt(rel.getRight());

    ResultSet resultSet;
    // 不传就是 *，选择全部的列
    if (selectedName.size() == 0)
        // 添加全部映射
        resultSet.setMapping(target->getColumns(), target->getColumnSize());
    else
        // 遍历添加映射
        for (const auto &item: selectedName)
            resultSet.appendMapping((*target)[item]);

    ColumnPtr col = target->selectColumn(leftOperand);

    Assert::nonNull(col, "at SqlExecutor::execSimpleQuery: can not find column called " + leftOperand);

    target->loadAllPage();

    // 遍历所有页面
    for (int i = 0; i < target->getPageSize(); ++i)
        // 获取当前页面的所有 tuple
        for (const auto &item: target->getPage(i)->getTuples())
            // 执行表达式，如果满足则加入结果集
            if (SqlExecutor::eval(col, item, op, rightOperand))
                resultSet.append(item);

    // 返回结果集
    return resultSet;
}

void SqlExecutor::setTables(std::unordered_map<int, TablePtr> *_tables) {
    this->tables = _tables;
}

bool SqlExecutor::eval(ColumnPtr column, const TupleData &tuple, BiOperator op, int rightOperand) {
    int leftOperand = *((int *) tuple.data[column->order]);
    switch (op) {
        case GE:
            return leftOperand >= rightOperand;
        case LE:
            return leftOperand <= rightOperand;
        case G:
            return leftOperand > rightOperand;
        case L:
            return leftOperand < rightOperand;
        case E:
            return leftOperand == rightOperand;
        case NE:
            return leftOperand != rightOperand;
        default:
            return false;
    }
}

#undef tableId
#undef table