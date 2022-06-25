#ifndef HDB_ARRAY_H
#define HDB_ARRAY_H

#include <iostream>
#include "ArraySlice.h"
#include "Assert.h"

#define _SIZE_OF_T(_size) (sizeof(T) * _size)
#define _MALLOC_SIZE_OF_T(_size) (T*)malloc(_SIZE_OF_T(_size))

#define _CHECK_OUT_OF_RANGE(_addCnt) \
    Assert::isFalse(length + _addCnt > size, "out of range")

template <class T>
class Array {
   public:
    Array()
        : length(0), size(Array::DEFAULT_SIZE), v(_MALLOC_SIZE_OF_T(size)) {}

    Array(int32_t initialSize)
        : length(0), size(initialSize), v(_MALLOC_SIZE_OF_T(size)) {}

    void append(T val) {
        _CHECK_OUT_OF_RANGE(1);

        v[length++] = val;
    }

    void append(int32_t idx, T val) {
        _CHECK_OUT_OF_RANGE(1);

        for (int i = length; i > idx + 1; i--)
            v[i] = v[i - 1];

        v[idx + 1] = val;
    }

    T pop() {
        Assert::isFalse(length == 0, "array is empty");
        return v[--length];
    }

    void remove(int32_t idx) {}

    void retire() {}

    void clear() {}

    ArraySlice<T> slice(int32_t start, int32_t end);

   private:
    T* v;
    int32_t length;
    int32_t size;

    const static int32_t DEFAULT_SIZE = 10;
};

#undef _MALLOC_SIZE_OF_T
#undef _TEMPLATE_T
#undef _SIZE_OF_T

#endif