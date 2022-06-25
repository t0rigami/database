#ifndef HDB_SEMAPHORE_H
#define HDB_SEMAPHORE_H

/**
 * @brief 定义信号量操作的一套接口
 *
 */
class Semaphore {
   public:
    /**
     * @brief 等待获取锁
     *
     * @return bool 是否获取成功
     */
    virtual bool wait() = 0;

    /**
     * @brief 释放锁
     *
     * @return bool 是否释放成功
     */
    virtual bool post() = 0;
};

typedef Semaphore* SemaphorePtr;

#endif
