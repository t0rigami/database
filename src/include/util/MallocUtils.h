#ifndef HDB_MALLOC_UTILS
#define HDB_MALLOC_UTILS

#include <cstdio>
#include <cstring>
#include <iostream>
#include "types.h"

#define MALLOC_SIZE_OF_TYPE(size, type) (type*)malloc(size * sizeof(type))

namespace MallocUtils {

    char *cString(const std::string &str);

    void *mallocAndInitial(size_t size);

    void *mallocAndInitial(size_t size, int init);

    void *mallocAndCopy(void *src, size_t size);

    void retire(void *p);

    void retire(byte_array byteArray);

    void resize(void **p, size_t size);

    byte_array mallocBytes(int size);

}  // namespace MallocUtils
#endif