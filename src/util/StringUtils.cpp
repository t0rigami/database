#include "StringUtils.h"
#include <algorithm>

std::vector<std::string> StringUtils::split(const std::string &s, char divide) {
    std::vector<std::string> result;

    size_t l = 0, r, len = s.size();

    while (l < len) {
        while (s[l] == divide && l < len)
            l++;

        r = l;

        while (r + 1 < len && s[r + 1] != divide)
            r++;

        if (s[r] != divide) {
            result.push_back(s.substr(l, r - l + 1));
        }
        l = r + 1;
    }

    return result;
}

bool StringUtils::isNotBlank(const std::string &s) {
    return std::any_of(s.begin(), s.end(), [](char ch) {
        return ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t';
    });
}

bool StringUtils::isBlank(const std::string &s) {
    return std::all_of(s.begin(), s.end(), [](char ch) {
        return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
    });
}

std::string StringUtils::toString(ColumnTypeEnum type) {
    switch (type) {
        case INT:
            return "INT";
        case CHAR:
            return "CHAR";
        case VARCHAR:
            return "VARCHAR";
        default:
            return "NONE";
    }
}

bool StringUtils::isNumber(const std::string &numStr) {
    try {
        std::stoll(numStr);
        return true;
    } catch (std::invalid_argument &e) {
        return false;
    }
}

int StringUtils::parseInt(const std::string &numStr) {
    return std::stoi(numStr);
}