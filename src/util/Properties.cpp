#include "Properties.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Assert.h"
#include "StringUtils.h"

using namespace std;

Properties::Properties(const std::string &path) {
    // 读取文件
    ifstream f(path);

    // 校验是否打开文件
    Assert::isTrue(f.is_open(), "cant not open the file " + path);

    // 每次读取一行
    string line;
    while (getline(f, line)) {
        // 根据分隔符分割
        auto keyValue = StringUtils::split(line, Properties::divide);

        // 判断配置文件是否合法
        Assert::equal(keyValue.size(), 2,
                      "find the file " + path + " is incorrect file format");

        // 合法则设置值
        set(keyValue[0], keyValue[1]);
    }
}

const std::string &Properties::get(const std::string &key) {
    return hashMap[key];
}

void Properties::set(const std::string &key, const std::string &value) {
    hashMap.insert({key, value});
}

void Properties::clear() {
    hashMap.clear();
}
