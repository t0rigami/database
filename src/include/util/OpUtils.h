//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_OP_UTILS_H
#define DB_SERVER_OP_UTILS_H

#include "BiOperator.h"
#include <vector>

namespace OpUtils {
    const std::vector<std::string> BI_OP{"+", "-", "*", "/", ">=", "<=", ">", "<", "=", "!="};

    BiOperator strToBiOp(const std::string &opStr);

    const std::string &biOpToStr(BiOperator op);

    BiOperator reverseBiOp(BiOperator op);
}

#endif //DB_SERVER_OP_UTILS_H
