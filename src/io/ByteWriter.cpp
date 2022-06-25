#include "ByteWriter.h"
#include <utility>
#include "Assert.h"
#include "IoUtils.h"
#include "MallocUtils.h"

#define _BUFFER buffer
#define _OFFSET offset
#define _BYTE_SIZE sizeof(byte)

#define _CHECK_BUF(_size) checkBuf(_size)

#define _WRITE_BASIC_TYPE(_src, _type)                   \
    size_t _type_size = sizeof(_type);                   \
    _CHECK_BUF(_type_size);                 \
    memcpy(_BUFFER + _OFFSET, (byte*)&_src, _type_size); \
    _OFFSET += _type_size

#define _WRITE_REF_TYPE(_src, _size)        \
    _CHECK_BUF(_size);         \
    memcpy(_BUFFER + _OFFSET, _src, _size); \
    _OFFSET += _size

const size_t ByteWriter::DEFAULT_BUFFER_SIZE = 4096;

ByteWriter::ByteWriter()
        : size(ByteWriter::DEFAULT_BUFFER_SIZE),
          offset(0),
          buffer((byte *) MallocUtils::mallocAndInitial(
                  ByteWriter::DEFAULT_BUFFER_SIZE)),
          autoRetire(false),
          isOpen(false),
          f(nullptr) {}

ByteWriter::ByteWriter(size_t bufSize)
        : size(bufSize),
          buffer((byte *) MallocUtils::mallocAndInitial(bufSize)),
          autoRetire(false),
          isOpen(false),
          offset(0),
          f(nullptr) {}

ByteWriter::ByteWriter(std::string destPath)
        : destPath(std::move(destPath)),
          size(ByteWriter::DEFAULT_BUFFER_SIZE),
          offset(0),
          buffer((byte *) MallocUtils::mallocAndInitial(
                  ByteWriter::DEFAULT_BUFFER_SIZE)),
          autoRetire(true),
          isOpen(false),
          f(nullptr) {}

ByteWriter::ByteWriter(std::string destPath, size_t size)
        : destPath(std::move(destPath)),
          size(size),
          offset(0),
          buffer((byte *) MallocUtils::mallocAndInitial(size)),
          autoRetire(true),
          isOpen(false),
          f(nullptr) {}

ByteWriter::~ByteWriter() {
    if (autoRetire) {
        MallocUtils::retire(buffer);
    }
}

void ByteWriter::writeInt(const int32_t &n) {
    _WRITE_BASIC_TYPE(n, int32_t);
}

void ByteWriter::writeUInt(const uint32_t &n) {
    _WRITE_BASIC_TYPE(n, int);
}

void ByteWriter::writeLong(const int64_t &n) {
    _WRITE_BASIC_TYPE(n, int64_t);
}

void ByteWriter::writeULong(const uint64_t &n) {
    _WRITE_BASIC_TYPE(n, uint64_t);
}

void ByteWriter::writeChar(const char &ch) {
    _WRITE_BASIC_TYPE(ch, char);
}

void ByteWriter::writeString(const std::string &str) {
    const char *s = str.c_str();
    size_t str_size = (str.size() + 1) * sizeof(char);
    writeULong(str_size);
    _WRITE_REF_TYPE(s, str_size);
}

void ByteWriter::writeString(const char *str) {
    size_t str_size = (strlen(str) + 1) * sizeof(char);
    writeULong(str_size);
    _WRITE_REF_TYPE(str, str_size);
}

void ByteWriter::writeObject(const Serializable *obj) {
    byte_array bytes = obj->serialize();
    byte_array encoding = IoUtils::encode(bytes);

    // 先写入当前对象的大小
    writeULong(encoding.size);
    // 再写入对象
    _WRITE_REF_TYPE(encoding.value, encoding.size);

    MallocUtils::retire(bytes);
    MallocUtils::retire(encoding);
}

void ByteWriter::writeBytes(const byte_array bytes) {
    writeULong(bytes.size);
    _WRITE_REF_TYPE(bytes.value, bytes.size);
}

void ByteWriter::writeEnd() {
    writeChar(IoUtils::END);
}

void ByteWriter::write(const byte *bytes, size_t size) {
    _WRITE_REF_TYPE(bytes, size);
//    printf("write _offset: %lu, _size: %lu, writeSize: %lu\n", offset, this->size,
//           size);
}

void ByteWriter::write(const byte_array &bytes) {
    write(bytes.value, bytes.size);
}

void ByteWriter::flush() {
    open();
    if (_OFFSET == 0) return;
    fwrite(_BUFFER, 1, _OFFSET, f);
    fflush(f);
    _OFFSET = 0;
}

void ByteWriter::close() {
    flush();
    fclose(this->f);
}

byte_array ByteWriter::getBuffer() {
    return {_BUFFER, _OFFSET};
}

void ByteWriter::checkBuf(size_t appendSize) {
    if (appendSize > size) {
        flush();
        while (size < appendSize)
            size *= 2;
        MallocUtils::resize((void **) &buffer, size);
    } else if (offset + appendSize > size) {
        flush();
    }
}

void ByteWriter::open() {
    Assert::isFalse(destPath.empty(), "not save Path!");

    if (isOpen) return;
    this->f = fopen(destPath.c_str(), "wb");
    Assert::isTrue(this->f != nullptr, "can not open the file: " + destPath);
    isOpen = true;
}

void ByteWriter::goPos(size_t pos) {
    flush();
    Assert::isTrue(fseek(this->f, pos, 0) == 0, "can not seek pos");
}

void ByteWriter::setOffset(size_t _offset) {
    this->offset = _offset;
}

void ByteWriter::setOffsetEnd() {
    this->offset = this->size;
}

size_t ByteWriter::getOffset() const {
    return offset;
}

#undef _WRITE_BASIC_TYPE
#undef _WRITE_REF_TYPE
#undef _SPACE
#undef _OFFSET