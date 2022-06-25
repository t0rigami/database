#include "IoUtils.h"

#include <vector>
#include "MallocUtils.h"

#define _VALUE data.value
#define _SIZE data.size

byte_array IoUtils::encode(const byte_array& data) {
    std::vector<byte> encodingData;

    for (size_t i = 0; i < _SIZE; i++)
        // 如果当前的 value 是 END 结束标志
        if (_VALUE[i] == IoUtils::END)
            // 结束标志乘 2
            encodingData.push_back(IoUtils::END),
                encodingData.push_back(IoUtils::END);
        else
            // 否则直接插入即可
            encodingData.push_back(_VALUE[i]);

    // 大小
    size_t size = encodingData.size() * sizeof(byte);

    // 拷贝并返回
    return {(byte*)MallocUtils::mallocAndCopy(&encodingData[0], size), size};
}

byte_array IoUtils::decode(const byte_array& data) {
    std::vector<byte> decodingData;

    for (size_t i = 0; i < _SIZE; i++)
        // 如果出现两个连续的结束标志，缩为一个
        if (i + 1 < _SIZE && _VALUE[i] == _VALUE[i + 1] &&
            _VALUE[i] == IoUtils::END)
            decodingData.push_back(IoUtils::END), i++;
        else
            decodingData.push_back(_VALUE[i]);

    size_t size = decodingData.size() * sizeof(byte);

    return {(byte*)MallocUtils::mallocAndCopy(&decodingData[0], size), size};
}

#undef _VALUE
#undef _SIZE