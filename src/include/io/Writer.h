#ifndef HDB_WRITER_H
#define HDB_WRITER_H

#include <iostream>
#include "Serializable.h"

class Writer {
   public:
    /**
     * @brief 写入一个 int32
     *
     * @param n int32
     */
    virtual void writeInt(const int32_t& n) = 0;
    /**
     * @brief 写入一个 uint32
     *
     * @param n uint32
     */
    virtual void writeUInt(const uint32_t& n) = 0;
    /**
     * @brief 写入一个 int64
     *
     * @param n int64
     */
    virtual void writeLong(const int64_t& n) = 0;
    /**
     * @brief 写入一个 uint64
     *
     * @param n uint64
     */
    virtual void writeULong(const uint64_t& n) = 0;
    /**
     * @brief 写入一个字符
     *
     * @param ch 字符
     */
    virtual void writeChar(const char& ch) = 0;
    /**
     * @brief 写入一个 C++ 的字符串
     *
     * @param str C++ 字符串
     */
    virtual void writeString(const std::string& str) = 0;
    /**
     * @brief 写入 C 语言风格的字符串
     *
     * @param str C 语言风格字符串
     */
    virtual void writeString(const char* str) = 0;
    /**
     * @brief 写入一个可序列化的对象
     *
     * @param obj 可序列化对象
     */
    virtual void writeObject(const Serializable* obj) = 0;
    /**
     * @brief 写入一些字节
     *
     * @param bytes 一些字节
     */
    virtual void writeBytes(byte_array bytes) = 0;
    /**
     * @brief 写入 end
     *
     */
    virtual void writeEnd() = 0;
    /**
     * @brief 写入字节
     *
     */
    virtual void write(const byte* bytes, size_t size) = 0;
    /**
     * @brief 写入字节数组
     *
     */
    virtual void write(const byte_array& bytes) = 0;
    /**
     * @brief 转移写指针到指定位置
     *
     * @param pos
     */
    virtual void goPos(size_t pos) = 0;
    /**
     * @brief 刷新缓冲区
     *
     */
    virtual void flush() = 0;
    /**
     * @brief 关闭流
     *
     */
    virtual void close() = 0;

    ~Writer() = default;
};

#endif