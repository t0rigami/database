#ifndef HDB_SERIALIZABLE_H
#define HDB_SERIALIZABLE_H

#include "types.h"

class Serializable {
   public:
    /**
     * @brief 序列化方法
     *
     * @return byte*
     */
    virtual byte_array serialize() const = 0;
};

#endif