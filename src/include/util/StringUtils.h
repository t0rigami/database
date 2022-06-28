#ifndef HDB_STRING_UTILS_H
#define HDB_STRING_UTILS_H

#include <iostream>
#include <vector>
#include "Column.h"
#include "BiOperator.h"

namespace StringUtils {

    std::vector<std::string> split(const std::string &s, char divide);

    bool isNotBlank(const std::string &s);

    bool isBlank(const std::string &s);

    std::string toString(ColumnTypeEnum type);

    bool isNumber(const std::string &numStr);

    int parseInt(const std::string &numStr);

} // namespace StringUtils

#endif  // HDB_STRING_UTILS_H
