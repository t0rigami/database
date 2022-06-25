#ifndef HDB_COLUMN
#define HDB_COLUMN

#include <cstdio>
#include "types.h"

struct Column;
typedef Column *ColumnPtr;

/**
 * @brief 列信息
 *
 */
struct Column {
    /**
     * @brief 当前列的名称
     *
     */
    char *name;
    /**
     * @brief 类型
     *
     */
    ColumnTypeEnum type;
    /**
     * @brief 所占空间大小
     *
     */
    int size;
    /**
     * @brief 排序
     *
     */
    int order;
    /**
     * 是否允许为空
     */
    bool nullable;
};

#endif