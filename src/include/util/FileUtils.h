//
// Created by Origami on 2022/6/23.
//

#ifndef DB_SERVER_FILE_UTILS_H
#define DB_SERVER_FILE_UTILS_H

#include <iostream>

namespace FileUtils {

    bool exists(const std::string &filePath);

    int size(const std::string &path);
}

#endif //DB_SERVER_FILE_UTILS_H
