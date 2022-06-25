#ifndef HDB_EXPER05_PROCESS
#define HDB_EXPER05_PROCESS

#include "ForkProcess.h"
#include "Semaphore.h"
// 废弃
class Exper05Process : public ForkProcess {
   public:
    Exper05Process(SemaphorePtr sem);

   private:
    void _run();
    SemaphorePtr sem;
};

#endif