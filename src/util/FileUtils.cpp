//
// Created by Origami on 2022/6/23.
//
#include "FileUtils.h"
#include <sys/stat.h>
#include "Assert.h"

bool FileUtils::exists(const std::string& filePath) {
    struct stat buffer {};
    return (stat(filePath.c_str(), &buffer) == 0);
}

int FileUtils::size(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    if (f == nullptr) return 0;
    fseek(f, 0, SEEK_END);
    int size = (int)ftell(f);
    fclose(f);
    return size;
}