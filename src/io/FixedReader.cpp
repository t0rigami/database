//
// Created by Origami on 2022/6/28.
//
#include "FixedReader.h"
#include "MallocUtils.h"
#include "Assert.h"

#define _READ_BASIC_TYPE(_type)             \
    _type _VALUE;                           \
    size_t _SIZE = sizeof(_type);           \
    checkBuf((int)_SIZE);                   \
    memcpy(&_VALUE, _buf + _offset, _SIZE); \
    _offset += _SIZE

#define _REF_SIZE readULong()

#define _READ_REF_TYPE(_type)                                     \
    size_t _SIZE = _REF_SIZE;                                     \
    checkBuf((int)_SIZE);                                         \
    _type* _VALUE = (_type*)MallocUtils::mallocAndInitial(_SIZE); \
    memcpy(&_VALUE, _buf + _offset, _SIZE);                       \
    _offset += _SIZE

FixedReader::FixedReader(int size, const std::string &path) : size(size),
                                                              _buf((byte *) MallocUtils::mallocAndInitial(size)),
                                                              _offset(0),
                                                              path(path) {
    f = fopen(path.c_str(), "rb");
    Assert::isTrue(f != nullptr,
                   "can not open the file : " + path);
    this->size = (int) fread(_buf, 1, this->size, f);
}

byte_array FixedReader::read(size_t _size) {
    Assert::isTrue(_offset + _size < this->size, "out of range!");
    auto data = MallocUtils::mallocAndCopy(_buf + _offset, _size);
    _offset += (int) _size;
    return {(byte *) data, _size};
}

int32_t FixedReader::readInt() {
    _READ_BASIC_TYPE(int32_t);
    return _VALUE;
}

void FixedReader::checkBuf(int nextSize) {
    if (_offset + nextSize > size) {
        // 缓存区读完了
        // 将后面的拷贝到前面
        int remainSize = size - _offset;
        memcpy(_buf, _buf + _offset, remainSize);
        // 然后再读取这么多
        int newSize = (int) fread(_buf + remainSize, 1, size - remainSize, f);
        if (newSize < size - remainSize) {
            size = newSize;
        }
        // offset 移到最前面
        _offset = 0;
    }
}

uint32_t FixedReader::readUInt() {
    _READ_BASIC_TYPE(uint32_t);
    return _VALUE;
}

int64_t FixedReader::readLong() {
    _READ_BASIC_TYPE(int64_t);
    return _VALUE;
}

uint64_t FixedReader::readULong() {
    _READ_BASIC_TYPE(uint64_t);
    return _VALUE;
}

char FixedReader::readChar() {
    _READ_BASIC_TYPE(char);
    return _VALUE;
}

std::string FixedReader::readString() {
    _READ_REF_TYPE(char);
    std::string str(_VALUE);
    MallocUtils::retire(_VALUE);
    return str;
}

char *FixedReader::readCString(size_t &strSize) {
    _READ_REF_TYPE(char);
    strSize = _SIZE;
    return _VALUE;
}

char *FixedReader::readCString() {
    _READ_REF_TYPE(char);
    return _VALUE;
}

byte_array FixedReader::readObjectByteArray() {
    size_t _SIZE = _REF_SIZE;
    printf("read size=%ld\n", _SIZE);
    checkBuf((int) _SIZE);
    byte *_VALUE = (byte *) MallocUtils::mallocAndInitial(_SIZE);
    memcpy(_VALUE, _buf + _offset, _SIZE);
    _offset += _SIZE;
    return {_VALUE, _SIZE};
}

void FixedReader::close() {
    fclose(f);
    MallocUtils::retire(_buf);
}

void FixedReader::goPos(size_t pos) {
    this->_offset = (int) pos;
}

bool FixedReader::hasNext() {
    return _offset < size || !feof(f);
}

const std::string &FixedReader::getPath() const {
    return path;
}

