#ifndef HDB_DEBUG_H
#define HDB_DEBUG_H
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include "Functional.h"
#include "types.h"
using namespace std;

/**
 * @brief 提供一些比较方便的 debug 方法
 *
 */
namespace Debug {

/**
 * @brief 打印
 *
 */
namespace Print {

const size_t ROW_OF_BYTES_LENGTH = 20;

enum PrintOption {
    DEFAULT = 0,
    PID = (1 << 0),
    PID_SAMPLE = (1 << 1),
    TIMESTAMP = (1 << 2),
    TIMESTAMP_SAMPLE = (1 << 3),
};

void message(const char* msg, int options = PrintOption::DEFAULT);
void message(const std::string& msg, int options = PrintOption::DEFAULT);

void messageWithIdentity(const char* identity,
                         const char* msg,
                         int options = PrintOption::DEFAULT);

void messageWithIdentity(const std::string& identity,
                         const std::string& msg,
                         int options = PrintOption::DEFAULT);

void bytes(byte* bytes, size_t size);
void bytes(byte_array byteArray);
}  // namespace Print

}  // namespace Debug

#endif