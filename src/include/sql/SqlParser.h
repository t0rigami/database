//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_SQL_PARSER_H
#define DB_SERVER_SQL_PARSER_H

#include "BiRelation.h"
#include "Optional.h"

class SqlParser {
public:
    /**
     * 将 A + B, A = B，等转换为二元关系
     * @param expression 表达式字符串
     * @return 二元关系
     */
    static Optional<BiRelation> toBiRelation(const std::string &expression);
};

#endif //DB_SERVER_SQL_PARSER_H
