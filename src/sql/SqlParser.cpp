//
// Created by Origami on 2022/6/27.
//
#include "SqlParser.h"
#include "StringUtils.h"
#include <OpUtils.h>

Optional<BiRelation> SqlParser::toBiRelation(const std::string &expression) {
    /**
     * 只有满足 [A op B] 格式才解析成功
     */

    std::vector<std::string> splits = StringUtils::split(expression, ' ');

    if (splits.size() != 3) return Optional<BiRelation>();

    BiOperator op = OpUtils::strToBiOp(splits[1]);

    if ((int) op == -1) return Optional<BiRelation>();

    return Optional<BiRelation>(BiRelation(splits[0], splits[2], op));
}
