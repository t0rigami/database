#include "Assert.h"
#include <iostream>

void Assert::between(size_t _val, size_t l, size_t r) {
    if (l > _val || _val < r)
        throw std::logic_error(std::to_string(r) + "is not between range " +
                               std::to_string(l) + " of " + std::to_string(r));
}

void Assert::equal(size_t _val, size_t expect) {
    if (_val != expect)
        throw std::logic_error(
                "value: " + std::to_string(_val) +
                " is not equal expect: " + std::to_string(expect));
}

void Assert::equal(size_t _val, size_t expect, const std::string &error_cause) {
    if (_val != expect) throw std::logic_error(error_cause);
}

void Assert::isTrue(bool exp) {
    if (!exp) throw std::logic_error("except true but false");
}

void Assert::isTrue(bool exp, const std::string &error_cause) {
    if (!exp) throw std::logic_error(error_cause);
}

void Assert::isFalse(bool exp) {
    if (exp) throw std::logic_error("except false but true");
}

void Assert::isFalse(bool exp, const std::string &error_cause) {
    if (exp) throw std::logic_error(error_cause);
}

void Assert::nonNull(void *p, const std::string &msg) {
    if (p == nullptr) throw std::logic_error(msg);
}