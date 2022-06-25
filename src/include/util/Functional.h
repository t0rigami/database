#ifndef HDB_FUNCTIONAL_H
#define HDB_FUNCTIONAL_H
/**
 * @brief 声明一些函数式接口
 *
 */
namespace Functional {

#define CONSUMER(type) void (*consumer)(type value)
#define RUNABLE void (*run)()

}  // namespace Functional

#endif