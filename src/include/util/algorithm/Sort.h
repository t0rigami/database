//
// Created by Origami on 2022/6/28.
//

#ifndef DB_SERVER_SORT_H
#define DB_SERVER_SORT_H

#include "Table.h"
#include "FixedReader.h"

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


    /**
     * 将表的所有记录进行外排序
     * @param table 表信息
     * @param destPath 表的目标地址
     */
    template<typename Compare>
    static void externalSort(
            TablePtr table,
            const std::string &destPath,
            const std::string &tempPath,
            Compare cmp
    ) {
        /*
         * 实现方式，先将每一个块进行排序
         * 然后归并排序
         */

        // 待排序的文件
        std::vector<std::string> unmerge;

        // 逐一读取页面
        for (int i = 0; i < table->getPageSize(); ++i) {
            PagePtr page = table->getPage(i);

            // 对页面进行排序，并将排序完的结果返回
            std::vector<TupleData> sortedTemp = Sort::quickSort(page->getTuples(), cmp);

            auto path = tempPath + "/" + std::to_string(i) + ".tp";
            // 将排序完的结果保存到临时文件中
            TupleData::listSave(sortedTemp, path);

            // 将此路径加入到待合并的文件中
            unmerge.push_back(path);
        }

        // 当待合并的文件数超过 1 个时，进入循环
        while (unmerge.size() > 1) {
            // 从里面拿出最后一个元素
            std::string rightPath = unmerge.back();
            // 弹出最后一个元素
            unmerge.pop_back();

            // 读取这两个文件
            FixedReader left(512, unmerge.back());
            FixedReader right(512, rightPath);

            // 进入 merge 流程
            _merge(left, right, cmp);
        }

        // 最后将最后一个文件重命名为目标文件
        rename(unmerge.back().c_str(), destPath.c_str());
    }

private:
    template<typename Compare>
    static void _merge(FixedReader &leftReader, FixedReader &rightReader, Compare cmp) {
        // 准备一个临时路径
        const std::string temp = "./tmp/t.tp";

        // 准备往临时路径中写入文件
        ByteWriter bw(temp);

        // 定义一个缓存结构体，防止多次解析
        struct Cache {
            TupleData tuple{};
            byte_array bytes{};
            bool hit{false};
        } leftCache, rightCache;

        /*
         * 当 l 缓存中有元素或者还可以读出元素
         * 并且 r 缓存中有元素或者还可以读取元素
         * 时继续循环
         */
        while ((leftCache.hit || leftReader.hasNext()) && (rightCache.hit || rightReader.hasNext())) {
            // 如果缓存中没有，则读取一个元素加入到缓存
            if (!leftCache.hit) {
                leftCache.bytes = leftReader.readObjectByteArray();
                leftCache.tuple = TupleData::fromBytes(leftCache.bytes);
                leftCache.hit = true;
            }
            if (!rightCache.hit) {
                rightCache.bytes = rightReader.readObjectByteArray();
                rightCache.tuple = TupleData::fromBytes(rightCache.bytes);
                rightCache.hit = true;
            }

            // 进入比较阶段，根据比较规则
            if (cmp(leftCache.tuple, rightCache.tuple) >= 0) {
                // 写入 rightCache，并设置 r 缓存失效
                bw.write(rightCache.bytes);
                MallocUtils::retire(rightCache.bytes);
                rightCache.hit = false;
            } else {
                // 写入 leftCache，并设置 l 缓存失效
                bw.write(leftCache.bytes);
                MallocUtils::retire(leftCache.bytes);
                leftCache.hit = false;
            }
        }

        // 当 l 还有元素
        while (leftCache.hit || leftReader.hasNext()) {
            byte_array bytes{};
            if (leftCache.hit) {
                // 如果是缓存中的元素，直接拿出来
                bytes = leftCache.bytes;
                leftCache.hit = false;
            } else {
                // 否则就继续读取
                bytes = leftReader.readObjectByteArray();
            }
            bw.write(bytes);
            MallocUtils::retire(bytes);
        }

        // 当 r 还有元素
        while (rightCache.hit || rightReader.hasNext()) {
            byte_array bytes{};
            if (rightCache.hit) {
                // 如果是缓存中的元素，直接拿出来
                bytes = rightCache.bytes;
                rightCache.hit = false;
            } else {
                // 否则就继续读取
                bytes = rightReader.readObjectByteArray();
            }
            bw.write(bytes);
            MallocUtils::retire(bytes);
        }

        // 关闭所有资源
        leftReader.close();
        rightReader.close();
        bw.close();

        // 移除临时文件 l 和 r
        remove(leftReader.getPath().c_str());
        remove(rightReader.getPath().c_str());
        // 将临时合并的文件改名为 l 的名称
        rename(temp.c_str(), leftReader.getPath().c_str());
        // 合并结束！
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
};

#endif //DB_SERVER_SORT_H
