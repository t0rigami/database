#include "MallocUtils.h"

char *MallocUtils::cString(const std::string &str) {
    const char *src = str.c_str();
    size_t srcSize = strlen(src) * sizeof(char);

    char *result = (char *) malloc(srcSize + 1);
    strcpy(result, src);
    return result;
}

void *MallocUtils::mallocAndInitial(size_t size) {
    return mallocAndInitial(size, 0);
}

void *MallocUtils::mallocAndInitial(size_t size, int init) {
    void *res = malloc(size);
    memset(res, init, size);
#ifdef MALLOC_UTILS_DEBUG
    printf("malloc address:[%p] _size:[%lu]\n", res, _size);
#endif
    return res;
}

void *MallocUtils::mallocAndCopy(void *src, size_t size) {
    void *dest = malloc(size);
    memcpy(dest, src, size);
    return dest;
}

void MallocUtils::retire(void *p) {
#ifdef MALLOC_UTILS_DEBUG
    printf("free address:[%p]\n", p);
#endif

    free(p);
}

void MallocUtils::retire(byte_array arr) {
#ifdef MALLOC_UTILS_DEBUG
    printf("free address:[%p]\n", arr.value);
#endif

    free(arr.value);
}

void MallocUtils::resize(void **p, size_t size) {
    retire(*p);
    (*p) = mallocAndInitial(size);
}

byte_array MallocUtils::mallocBytes(int size) {
    return {static_cast<byte *>(MallocUtils::mallocAndInitial(size)), size};
}