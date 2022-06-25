#ifndef HDB_ASSERT_H
#define HDB_ASSERT_H
#include <cstdlib>
#include <string>

namespace Assert {

void between(size_t _val, size_t l, size_t r);
void equal(size_t _val, size_t expect);
void equal(size_t _val, size_t expect, const std::string& error_cause);
void isTrue(bool exp);
void isTrue(bool exp, const std::string& error_cause);
void isFalse(bool exp);
void isFalse(bool exp, const std::string& error_cause);

}  // namespace Assert

#endif  // HDB_ASSERT_H