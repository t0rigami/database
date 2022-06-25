#ifndef HDB_PROPERTIES_H
#define HDB_PROPERTIES_H

#include <iostream>
#include <unordered_map>

class Properties {
public:
    /**
     * @brief 从文件路径中读取配置文件
     *
     * @param path 文件路径
     */
    explicit Properties(const std::string &path);

    /**
     * @brief 根据 key 获取一个值
     *
     * @param key
     * @return std::string 对应的值，若不存在返回空
     */
    const std::string &get(const std::string &key);

    /**
     * @brief 设置一个键值对
     *
     * @param key 建
     * @param value 值
     */
    void set(const std::string &key, const std::string &value);

    /**
     * @brief 清空所有键值
     *
     */
    void clear();

    /**
     * @brief 配置文件的分隔符
     *
     */
    static const char divide = '=';

private:
    /**
     * @brief 内部数据结构
     *
     */
    std::unordered_map<std::string, std::string> hashMap;
};

#endif  // !HDB_PROPERTIES_H
