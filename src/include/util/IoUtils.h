#ifndef HDB_IO_UTILS_H
#define HDB_IO_UTILS_H

#include "types.h"

namespace IoUtils {

/**
 * @brief 分隔符
 *
 */
const byte END = -1;

/**
 * @brief 编码
 *
 * @param data
 * @return byte*
 */
byte_array encode(const byte_array& data);
/**
 * @brief 解码
 *
 * @param data
 * @return byte*
 */
byte_array decode(const byte_array& data);

}  // namespace IoUtils

#endif
