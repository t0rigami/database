//
// Created by Origami on 2022/6/23.
//
#include "FileUtils.h"
#include "Assert.h"
#include <sys/stat.h>

bool FileUtils::exists(const std::string &filePath) {
    struct stat buffer{};
    return (stat(filePath.c_str(), &buffer) == 0);
}

int FileUtils::size(const std::string &path) {
    FILE *f = fopen(path.c_str(), "rb");
    Assert::isFalse(f == nullptr, "can not open file");
    fseek(f, 0, SEEK_END);
    int size = (int) ftell(f);
    fclose(f);
    return size;
}