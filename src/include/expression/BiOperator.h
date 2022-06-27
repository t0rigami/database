//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_BIOPERATOR_H
#define DB_SERVER_BIOPERATOR_H

#include <iostream>

/**
 * 操作符枚举
 */
enum BiOperator {
    /**
     * 加
     */
    ADD = 0,
    /**
     * 减
     */
    SUB = 1,
    /**
     * 乘
     */
    MUL = 2,
    /**
     * 除
     */
    DIV = 3,
    /**
     * 大于等于
     */
    GE = 4,
    /**
     * 小于等于
     */
    LE = 5,
    /**
     * 大于
     */
    G = 6,
    /**
     * 小于
     */
    L = 7,
    /**
     * 等于
     */
    E = 8,
    /**
     * 不等于
     */
    NE = 9
};

#endif //DB_SERVER_BIOPERATOR_H
