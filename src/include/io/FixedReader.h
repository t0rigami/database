//
// Created by Origami on 2022/6/28.
//

#ifndef DB_SERVER_FIXED_WRITER_H
#define DB_SERVER_FIXED_WRITER_H

#include "Reader.h"

class FixedReader : public Reader {
public:
    FixedReader(int size, const std::string &path);

    byte_array read(size_t _size) override;

    int32_t readInt() override;

    uint32_t readUInt() override;

    int64_t readLong() override;

    uint64_t readULong() override;

    char readChar() override;

    std::string readString() override;

    char *readCString(size_t &strSize) override;

    char *readCString() override;

    byte_array readObjectByteArray() override;

    void close() override;

    void goPos(size_t pos) override;

    void checkBuf(int nextSize);

    bool hasNext();

    const std::string &getPath() const;

private:
    int size;
    int _offset;
    byte *_buf;
    FILE *f;
    std::string path;
};

#endif //DB_SERVER_FIXED_WRITER_H
