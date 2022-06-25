//
// Created by Origami on 2022/6/24.
//

#ifndef DB_SERVER_BUFFER_READER_H
#define DB_SERVER_BUFFER_READER_H

#include "Reader.h"

class BufferReader : public Reader {
public:
    explicit BufferReader(byte_array bytes);

    byte_array read(size_t size) override;

    int32_t readInt() override;

    uint32_t readUInt() override;

    int64_t readLong() override;

    uint64_t readULong() override;

    char readChar() override;

    std::string readString() override;

    char *readCString(size_t &size) override;

    char *readCString() override;

    byte_array readObjectByteArray() override;

    void close() override;

    void goPos(size_t pos) override;

    size_t getOffset() const;

private:
    byte *_buf;
    size_t _offset;
    size_t _size;
};

#endif //DB_SERVER_BUFFER_READER_H
