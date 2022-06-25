#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Debug.h"
#include "Semaphore.h"
#include "SemaphorePOSIX.h"
#include "SemaphoreXSI.h"

using namespace Debug::Print;
namespace Record {

// 信号量的测试
void semTest() {
    int option = PrintOption::PID_SAMPLE | PrintOption::TIMESTAMP_SAMPLE;
    SemaphorePtr sem1 = new SemaphorePOSIX();
    // 主进程 wait
    messageWithIdentity("Main", "start wait ..", option);
    sem1->wait();
    // fork 一个子进程
    pid_t pid = fork();

    if (pid < 0) {
        throw std::logic_error("fail fork ..");
    } else if (pid == 0) {
        // 子进程
        messageWithIdentity("Sub", "start wait ..", option);
        sem1->wait();
        messageWithIdentity("Sub", "do some thing ..", option);
        sleep(2);
        messageWithIdentity("Sub", "post ..", option);
    } else {
        messageWithIdentity("Main", "do some thing ..", option);
        // 父进程
        sleep(5);
        messageWithIdentity("Main", "post ..", option);
        // 释放 sem
        sem1->post();

        wait(&pid);
        messageWithIdentity("Main", "program end ..", option);
    }
}

/**
 * @brief 死锁的现象
 *
 */
void deadLockTest() {
    int option = PrintOption::PID_SAMPLE | PrintOption::TIMESTAMP_SAMPLE;

    SemaphorePtr sem1 = new SemaphoreXSI(5);
    SemaphorePtr sem2 = new SemaphoreXSI(6);
    // 父进程获取 sem1
    messageWithIdentity("Main", "want to get sem1 ..", option);
    sem1->wait();
    messageWithIdentity("Main", "get sem1 success ..", option);

    pid_t pid = fork();
    if (pid < 0) {
        throw std::logic_error("fork fail");
    } else if (pid == 0) {
        // 子进程
        messageWithIdentity("Sub", "want to get sem2 ..", option);
        sem2->wait();
        messageWithIdentity("Sub", "get sem2 success ..", option);

        messageWithIdentity("Sub", "want to get sem1 ..", option);
        sem1->wait();
        messageWithIdentity("Sub", "get sem1 success ..", option);
        sem1->post();
        sem2->post();
    } else {
        // 父进程，等待子获取 sem2
        sleep(2);
        messageWithIdentity("Main", "want to get sem2 ..", option);
        sem2->wait();
        messageWithIdentity("Main", "get sem2 success ..", option);

        sem2->post();
        sem1->post();

        wait(&pid);
    }
    delete sem1;
    delete sem2;
}
}  // namespace Record
