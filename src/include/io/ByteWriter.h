#ifndef HDB_BYTE_WRITER
#define HDB_BYTE_WRITER

#include "Writer.h"

class ByteWriter;

typedef ByteWriter *ByteWriterPtr;

class ByteWriter : public Writer {
public:
    ByteWriter();

    explicit ByteWriter(size_t bufSize);

    explicit ByteWriter(std::string destPath);

    ByteWriter(std::string destPath, size_t size);

    void writeInt(const int32_t &n) override;

    void writeUInt(const uint32_t &n) override;

    void writeLong(const int64_t &n) override;

    void writeULong(const uint64_t &n) override;

    void writeChar(const char &ch) override;

    void writeString(const std::string &str) override;

    void writeString(const char *str) override;

    void writeObject(const Serializable *obj) override;

    void writeBytes(byte_array bytes) override;

    void write(const byte *bytes, size_t size) override;

    void write(const byte_array &bytes) override;

    void writeEnd() override;

    void goPos(size_t pos) override;

    void setOffset(size_t _offset);

    void setOffsetEnd();

    void flush() override;

    void close() override;

    byte_array getBuffer();

    ~ByteWriter();

    // 是否在销毁的时候自动回收 buffer
    bool autoRetire;

    size_t getOffset() const;

private:
    void open();

    void checkBuf(size_t appendSize);

    FILE *f;
    // buffer
    byte *buffer;
    // 当前写指针的偏移量
    size_t offset;
    // 当前 buffer 的大小
    size_t size;
    // 保存路径
    std::string destPath;

    bool isOpen;

    const static size_t DEFAULT_BUFFER_SIZE;
};

#endif