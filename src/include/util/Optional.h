#ifndef HDB_OPTIONAL_H
#define HDB_OPTIONAL_H

#include "Assert.h"
#include "Functional.h"

template <class T>
class Optional {
   public:
    /**
     * @brief 创建一个空的 Optional
     *
     */
    explicit Optional();
    /**
     * @brief 创建一个有值的 Optional
     *
     * @param value 当前值
     */
    explicit Optional(const T& value);

    /**
     * @brief 如果当前有值，就执行操作
     *
     * @param consumer 消费者
     */
    void ifPresent(CONSUMER(T));

    /**
     * @brief 如果有值，则，否则
     *
     */
    void ifPresentElse(CONSUMER(T), RUNABLE);

    /**
     * @brief 获取值，如果当前值为空，抛出异常
     *
     * @return 值
     */
    T get();

    /**
     * @brief 获取值，如果没有值则返回一个默认值
     *
     * @return T
     */
    T getOrElse(const T& defaultValue);

    /**
     * @brief 是否为空
     *
     * @return true 空
     * @return false 非空
     */
    bool empty() const;

   private:
    T value;
    bool isEmpty;
};

template <class T>
Optional<T>::Optional() : isEmpty(true) {}

template <class T>
Optional<T>::Optional(const T& value) : isEmpty(false), value(value) {}

template <class T>
void Optional<T>::ifPresent(CONSUMER(T)) {
    if (!isEmpty)
        consumer(value);
}

template <class T>
T Optional<T>::get() {
    Assert::isFalse(isEmpty, "null pointer");

    return value;
}

template <class T>
T Optional<T>::getOrElse(const T& defaultValue) {
    return isEmpty ? defaultValue : value;
}

template <class T>
void Optional<T>::ifPresentElse(CONSUMER(T), RUNABLE) {
    if (isEmpty)
        run();
    else
        consumer(value);
}

template <class T>
bool Optional<T>::empty() const {
    return isEmpty;
}

#endif