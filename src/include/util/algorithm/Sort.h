//
// Created by Origami on 2022/6/28.
//

#ifndef DB_SERVER_SORT_H
#define DB_SERVER_SORT_H

#include "Table.h"

class Sort {
public:

    /**
     * 对表的一个块
     * @param record 待排序的序列
     * @param cmp 排序方法，cmp(a, b)
     *              if a > b => > 0
     *              if a = b => = 0
     *              if a < b => < 0
     */
    template<typename Type, typename Compare>
    static std::vector<Type> quickSort(std::vector<Type> record, Compare cmp) {
        _quickSort(record, cmp, 0, (int) record.size() - 1);
        return record;
    }

    template<typename Type, typename Compare>
    static void _quickSort(std::vector<Type> &record, Compare cmp, int l, int r) {
        if (l >= r) return;

        int i = l - 1, j = r + 1;
        Type x = record[(l + r) >> 1];
        do {
            // 左边都小于 x
            do ++i; while (cmp(record[i], x) < 0);
            // 右边都大于 x
            do --j; while (cmp(record[j], x) > 0);

            if (i < j)
                std::swap(record[i], record[j]);

        } while (i < j);

        _quickSort(record, cmp, l, j);
        _quickSort(record, cmp, j + 1, r);
    }


    /**
     * 将表的所有记录进行外排序
     * @param table 表信息
     * @param destPath 表的目标地址
     */
    static void externalSort(TablePtr table, const std::string &destPath);
};

#endif //DB_SERVER_SORT_H
