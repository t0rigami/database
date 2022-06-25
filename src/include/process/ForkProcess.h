#ifndef HDB_FORK_PROCESS_H
#define HDB_FORK_PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

class ForkProcess {
   public:
    /**
     * @brief 运行 fork 出来的进程
     *
     */
    void run();

   protected:
    pid_t currentProcessId;
    /**
     * @brief 交由子类实现 run 方法
     *
     */
    virtual void _run() = 0;

   private:
};

#endif