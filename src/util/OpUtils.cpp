//
// Created by Origami on 2022/6/27.
//
#include "OpUtils.h"

BiOperator OpUtils::strToBiOp(const std::string &opStr) {
    for (int i = 0; i < BI_OP.size(); i++)
        if (BI_OP[i] == opStr)
            return (BiOperator) i;
    return (BiOperator) -1;
}

const std::string &OpUtils::biOpToStr(BiOperator op) {
    return BI_OP[int(op)];
}

BiOperator OpUtils::reverseBiOp(BiOperator op) {
    switch (op) {
        case DIV:
        case SUB:
            throw std::logic_error("not support reverse op!");
        case GE:
            return LE;
        case LE:
            return GE;
        case G:
            return L;
        case L:
            return G;
        default:
            return op;
    }
}