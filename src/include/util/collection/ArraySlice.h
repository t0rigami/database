#ifndef HDB_ARRAY_SLICE_H
#define HDB_ARRAY_SLICE_H

#include <iostream>

template <class T>
class ArraySlice {
   public:
   private:
    T* v;
    int32_t length;
};

#endif