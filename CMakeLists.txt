PROJECT(db_server)

cmake_minimum_required(VERSION 3.8)

# 添加 pthread 库
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
find_package(Threads)

# 头文件
include_directories(src/include)
include_directories(src/test)

# Table 源文件
include_directories(src/include/table)
aux_source_directory(src/table TABLE_SOURCE_DIR)

# Main 源文件
aux_source_directory(src/main MAIN_SOURCE_DIR)


# Util 源文件目录
include_directories(src/include/util)
aux_source_directory(src/util UTIL_SOURCE_DIR)

include_directories(src/include/util/collection)
aux_source_directory(src/util/collection COLLECTION_SOURCE_DIR)


# Access 源文件目录
aux_source_directory(src/access ACCESS_SOURCE_DIR)
# Conf 源文件目录
aux_source_directory(src/conf CONF_SOURCE_DIR)
# Component 源文件目录
include_directories(src/include/component)
aux_source_directory(src/component COMPONENT_SOURCE_DIR)

# net 源文件目录
include_directories(src/include/net)
aux_source_directory(src/net NET_SOURCE_DIR)

# process 源文件目录
include_directories(src/include/process)
aux_source_directory(src/process PROCESS_SOURCE_DIR)

# IO 源文件目录
include_directories(src/include/io)
aux_source_directory(src/io IO_SOURCE_DIR)

# SQL 源文件目录
include_directories(src/include/sql)
aux_source_directory(src/sql SQL_SOURCE_DIR)

# Expression 源文件目录
include_directories(src/include/expression)
aux_source_directory(src/expression EXPRESSION_SOURCE_DIR)

# Sort 源文件目录
include_directories(src/include/util/algorithm)
aux_source_directory(src/util/algorithm ALGORITHM_SOURCE_DIR)

# Client 源文件
aux_source_directory(src/client CLIENT_SOURCE_DIR)

# Test
include_directories(src/include/test)

aux_source_directory(src TOTAL_SOURCE_DIR)

add_executable(db_server ${MAIN_SOURCE_DIR} ${UTIL_SOURCE_DIR} ${ACCESS_SOURCE_DIR}
        ${CONF_SOURCE_DIR} ${COMPONENT_SOURCE_DIR} ${NET_SOURCE_DIR} ${PROCESS_SOURCE_DIR}
        ${THREAD_SOURCE_DIR} ${TEST_SOURCE_DIR} ${TABLE_SOURCE_DIR} ${IO_SOURCE_DIR} ${COLLECTION_SOURCE_DIR}
        ${SQL_SOURCE_DIR} ${EXPRESSION_SOURCE_DIR} ${ALGORITHM_SOURCE_DIR} ${TEST})

# add_executable (db_client ${CLIENT_SOURCE_DIR} ${UTIL_SOURCE_DIR} ${NET_SOURCE_DIR})

target_link_libraries(db_server ${CMAKE_THREAD_LIBS_INIT})