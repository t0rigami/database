#ifndef HDB_SEMAPHORE_POSIX_H
#define HDB_SEMAPHORE_POSIX_H

#include <semaphore.h>
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <unistd.h>
#include "Semaphore.h"

/**
 * @brief 基于 mmap 实现的信号量，
 * 无法在子进程被迫中止时释放相应的资源
 *
 */
class SemaphorePOSIX : public Semaphore {
public:
    explicit SemaphorePOSIX(int signalValue = 1);

    ~SemaphorePOSIX();

    /**
     * @brief 获取当前共享变量的值
     *
     * @return int
     */
    int value();

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
    sem_t *shareMemory;
};

typedef SemaphorePOSIX *SemaphorePOSIXPtr;

#endif
