#include "Exper05Process.h"

Exper05Process::Exper05Process(SemaphorePtr sem) : sem(sem) {}

void Exper05Process::_run() {
    std::cout << "Sub[" << currentProcessId << "]"
              << "wait start .." << std::endl;
    sem->wait();
    std::cout << "Sub[" << currentProcessId << "]"
              << "wait end .." << std::endl;
    // std::cout << "Sub[" << currentProcessId << "]"
    //           << "post end .." << std::endl;
}