//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_OPERATOR_H
#define DB_SERVER_OPERATOR_H

#include <iostream>

/**
 * 操作符枚举
 */
enum Operator {
    /**
     * 加
     */
    ADD,
    /**
     * 减
     */
    SUB,
    /**
     * 乘
     */
    MUL,
    /**
     * 除
     */
    DIV,
    /**
     * 大于等于
     */
    GE,
    /**
     * 小于等于
     */
    LE,
    /**
     * 大于
     */
    G,
    /**
     * 小于
     */
    L,
    /**
     * 等于
     */
    E,
};

#endif //DB_SERVER_OPERATOR_H
