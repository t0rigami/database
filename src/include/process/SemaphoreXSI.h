#ifndef HDB_SEMAPHORE_XSI_H
#define HDB_SEMAPHORE_XSI_H

#include <sys/sem.h>
#include "Semaphore.h"

class SemaphoreXSI : public Semaphore {
public:
    /**
     * @brief 操作信号量的联合体
     *
     */
    union semun {
        int val;
        semid_ds *buf;
        unsigned short *arry;
    };

    explicit SemaphoreXSI(int semId);

    ~SemaphoreXSI();

    /**
     * @brief 等待获取锁
     *
     * @return bool 是否获取成功
     */
    bool wait() override;

    /**
     * @brief 释放锁
     *
     * @return bool 是否释放成功
     */
    bool post() override;

private:
    /**
     * @brief 初始化信号量
     *
     * @param key 信号量的键值
     */
    inline void init(key_t key);

    /**
     * @brief 信号量描述符
     *
     */
    int semId;
};

typedef SemaphoreXSI *SemaphoreXSIPtr;

#endif