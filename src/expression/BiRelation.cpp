//
// Created by Origami on 2022/6/27.
//
#include "BiRelation.h"
#include "OpUtils.h"

BiRelation::BiRelation(const std::string &left, const std::string &right, BiOperator op) : left(left), right(right),
                                                                                           op(op) {}

void BiRelation::print() {
    printf("BiRelation{%s %s %s}\n", left.c_str(), OpUtils::biOpToStr(op).c_str(), right.c_str());
}

const std::string &BiRelation::getLeft() const {
    return left;
}

const std::string &BiRelation::getRight() const {
    return right;
}

BiOperator BiRelation::getOp() const {
    return op;
}

void BiRelation::swapOperand() {
    std::swap(this->left, this->right);
    op = OpUtils::reverseBiOp(op);
}

BiRelation::BiRelation() = default;
