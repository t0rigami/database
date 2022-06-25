#include "SemaphoreXSI.h"
#include "Assert.h"

SemaphoreXSI::SemaphoreXSI(int semId) : semId(semId) {
    // 初始化信号量
    init(semId);
}

void SemaphoreXSI::init(key_t key) {
    // 获取信号量
    if ((semId = semget(key, 1, 0640)) == -1) {
        // 如果信号量不存在，就创建它。
        Assert::isTrue(errno == 2, "fail init sem ..");

        Assert::isFalse(((semId = semget(key, 1, 0640 | IPC_CREAT)) == -1),
                        "fail create sem ..");

        // 信号量创建成功后，还需要把它初始化成可用的状态。
        semun sem_union;
        sem_union.val = 1;

        Assert::isFalse(semctl(semId, 0, SETVAL, sem_union) < 0,
                        "fail init sem ..");
    }
}

SemaphoreXSI::~SemaphoreXSI() {
    Assert::isFalse(semctl(semId, 0, IPC_RMID) == -1, "fail destory sem ..");
}

bool SemaphoreXSI::wait() {
    sembuf sem_b = {
        0,        // sem_num
        -1,       // sem_op
        SEM_UNDO  // sem_flg 使用 SEM_UNDO 操作系统会帮忙回收资源
    };
    return semop(semId, &sem_b, 1) != -1;
}

bool SemaphoreXSI::post() {
    sembuf sem_b = {
        0,        // sem_num
        1,        // sem_op
        SEM_UNDO  // sem_flg
    };
    return semop(semId, &sem_b, 1) != -1;
}
