//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_BI_RELATION_H
#define DB_SERVER_BI_RELATION_H

#include <iostream>
#include "BiOperator.h"

/**
 * 二元关系
 */
class BiRelation {
public:
    BiRelation();

    BiRelation(const std::string &left, const std::string &right, BiOperator op);

    /**
     * 打印到控制台
     */
    void print();

    /**
     * 获取左操作数
     * @return
     */
    const std::string &getLeft() const;

    /**
     * 获取右操作数
     * @return
     */
    const std::string &getRight() const;

    /**
     * 获取操作符
     * @return
     */
    BiOperator getOp() const;

    /**
     * 交换两个操作数
     */
    void swapOperand();

private:
    std::string left;
    std::string right;
    BiOperator op;
};

#endif //DB_SERVER_BI_RELATION_H
