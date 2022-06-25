#include "SemaphorePOSIX.h"
#include <sys/sem.h>
#include "Assert.h"

SemaphorePOSIX::SemaphorePOSIX(int signalValue) {
    // 创建一个共享内存 (可读可写) 匿名映射
    shareMemory = (sem_t*)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // 创建失败
    Assert::isFalse(shareMemory == NULL, "Share memory failed!");

    Assert::isFalse(sem_init(shareMemory, 1, signalValue) == -1,
                    "Init unamed sem failed!");
}

SemaphorePOSIX::~SemaphorePOSIX() {
    // 回收共享内存
    int destoryState = sem_destroy(shareMemory);
    Assert::isTrue(destoryState == 0, "destory sem failed");
}

int SemaphorePOSIX::value() {
    int val;
    sem_getvalue(shareMemory, &val);
    return val;
}

bool SemaphorePOSIX::wait() {
    return sem_wait(shareMemory) != -1;
}

bool SemaphorePOSIX::post() {
    return sem_post(shareMemory) != -1;
}