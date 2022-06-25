#ifndef HDB_BYTE_READER_H
#define HDB_BYTE_READER_H

#include <fstream>
#include "Reader.h"

class ByteReader : public Reader {
public:
    explicit ByteReader(std::string destPath);

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

private:
    void open();

    std::string destPath;
    FILE *f{};
    bool isOpen;
};

#endif