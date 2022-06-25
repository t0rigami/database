//
// Created by Origami on 2022/6/24.
//
#include "BufferReader.h"
#include "MallocUtils.h"
#include "Assert.h"

#define _READ_BASIC_TYPE(_type)             \
    _type _VALUE;                           \
    size_t _SIZE = sizeof(_type);           \
    memcpy(&_VALUE, _buf + _offset, _SIZE); \
    _offset += _SIZE

#define _REF_SIZE readULong()

#define _READ_REF_TYPE(_type)                                     \
    size_t _SIZE = _REF_SIZE;                                     \
    _type* _VALUE = (_type*)MallocUtils::mallocAndInitial(_SIZE); \
    memcpy(&_VALUE, _buf + _offset, _SIZE);                       \
    _offset += _SIZE

BufferReader::BufferReader(byte_array bytes) : _buf(bytes.value), _size(bytes.size), _offset(0) {
}

byte_array BufferReader::read(size_t size) {
    Assert::isTrue(_offset + size < _size, "out of range!");
    auto data = MallocUtils::mallocAndCopy(_buf + _offset, size);
    _offset += size;
    return {(byte *) data, size};
}

int32_t BufferReader::readInt() {
    _READ_BASIC_TYPE(int32_t);
    return _VALUE;
}

uint32_t BufferReader::readUInt() {
    _READ_BASIC_TYPE(uint32_t);
    return _VALUE;
}

int64_t BufferReader::readLong() {
    _READ_BASIC_TYPE(int64_t);
    return _VALUE;
}

uint64_t BufferReader::readULong() {
    _READ_BASIC_TYPE(uint64_t);
    return _VALUE;
}

char BufferReader::readChar() {
    _READ_BASIC_TYPE(char);
    return _VALUE;
}

std::string BufferReader::readString() {
    size_t refSize = _REF_SIZE;
    printf("refSize = %ld\n", refSize);
    auto str = (char *) MallocUtils::mallocAndCopy(_buf + _offset, refSize);
    std::string res = std::string(str);
    MallocUtils::retire(str);
    return res;
}

char *BufferReader::readCString(size_t &size) {
    size = _REF_SIZE;
    auto str = (char *) MallocUtils::mallocAndCopy(_buf + _offset, size);
    _offset += size;
    return str;
}

char *BufferReader::readCString() {
    size_t size = _REF_SIZE;
    auto str = (char *) MallocUtils::mallocAndCopy(_buf + _offset, size);
    _offset += size;
    return str;
}

byte_array BufferReader::readObjectByteArray() {
    _READ_REF_TYPE(byte);
    return {_VALUE, _SIZE};
}

void BufferReader::close() {
    MallocUtils::retire(_buf);
}

void BufferReader::goPos(size_t pos) {
    _offset = pos;
}

size_t BufferReader::getOffset() const {
    return _offset;
}

#undef _READ_BASIC_TYPE