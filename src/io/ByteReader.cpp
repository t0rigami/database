#include "ByteReader.h"

#include <utility>
#include "Assert.h"
#include "MallocUtils.h"

#define _VALUE _value
#define _SIZE _size
#define _REF_SIZE readULong()

#define _READ_BASIC_TYPE(_type)   \
    _type _VALUE;                 \
    size_t _SIZE = sizeof(_type); \
    fread(&_VALUE, 1, _SIZE, this->f)

#define _READ_REF_TYPE(_type)                                     \
    size_t _SIZE = _REF_SIZE;                                     \
    _type* _VALUE = (_type*)MallocUtils::mallocAndInitial(_SIZE); \
    fread(_VALUE, 1, _SIZE, this->f)

ByteReader::ByteReader(std::string destPath)
        : destPath(std::move(destPath)), isOpen(false), f(nullptr) {}

void ByteReader::open() {
    if (!isOpen) {
        this->f = fopen(destPath.c_str(), "rb");

        Assert::isTrue(f != nullptr,
                       "can not open the file : " + destPath);

        isOpen = true;
    }
}

byte_array ByteReader::read(size_t size) {
    byte *value = (byte *) MallocUtils::mallocAndInitial(size);
    fread(value, 1, size, f);
    return {value, size};
}

int32_t ByteReader::readInt() {
    open();
    _READ_BASIC_TYPE(int32_t);
    return _VALUE;
}

uint32_t ByteReader::readUInt() {
    open();
    _READ_BASIC_TYPE(uint32_t);
    return _VALUE;
}

int64_t ByteReader::readLong() {
    open();
    _READ_BASIC_TYPE(int64_t);
    return _VALUE;
}

uint64_t ByteReader::readULong() {
    open();
    _READ_BASIC_TYPE(uint64_t);
    return _VALUE;
}

char ByteReader::readChar() {
    open();
    _READ_BASIC_TYPE(char);
    return _VALUE;
}

std::string ByteReader::readString() {
    _READ_REF_TYPE(char);
    std::string str(_VALUE);
    MallocUtils::retire(_VALUE);

    return str;
}

char *ByteReader::readCString(size_t &size) {
    _READ_REF_TYPE(char);
    size = _SIZE;
    return _VALUE;
}

char *ByteReader::readCString() {
    _READ_REF_TYPE(char);
    return _VALUE;
}

byte_array ByteReader::readObjectByteArray() {
    _READ_REF_TYPE(byte);
    return {_VALUE, _SIZE};
}

void ByteReader::close() {
    fclose(f);
}

void ByteReader::goPos(size_t pos) {
    open();
    Assert::isTrue(fseek(f, (long) pos, 0) == 0, "can not seek");
}

#undef _REF_SIZE
#undef _READ_REF_TYPE
#undef _INPUT_STREAM
#undef _OFFSET
#undef _VALUE