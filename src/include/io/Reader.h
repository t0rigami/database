#ifndef HDB_READER_H
#define HDB_READER_H

#include <iostream>
#include "Serializable.h"

class Reader {
   public:
    virtual byte_array read(size_t size) = 0;
    virtual int32_t readInt() = 0;
    virtual uint32_t readUInt() = 0;
    virtual int64_t readLong() = 0;
    virtual uint64_t readULong() = 0;
    virtual char readChar() = 0;
    virtual std::string readString() = 0;
    virtual char* readCString(size_t& size) = 0;
    virtual char* readCString() = 0;
    virtual byte_array readObjectByteArray() = 0;
    virtual void close() = 0;
    virtual void goPos(size_t pos) = 0;
};

#endif