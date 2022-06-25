#include "ForkProcess.h"

void ForkProcess::run() {
    currentProcessId = fork();
    if (currentProcessId < 0)
        throw std::runtime_error("fork sub process failed");
    else if (currentProcessId == 0) {
        currentProcessId = getpid();
        // 子进程
        _run();
    } else {
        // 父进程
    }
}