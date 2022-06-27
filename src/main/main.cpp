#include <ctime>
#include <wait.h>
#include <iostream>
#include "Array.h"
#include "ByteReader.h"
#include "ByteWriter.h"
#include "DatabaseContext.h"
#include "Debug.h"
#include "IoUtils.h"
#include "Semaphore.h"
#include "SemaphorePOSIX.h"
#include "SemaphoreXSI.h"
#include "ServerSocket.h"
#include "Table.h"
#include "TupleData.h"
#include "SqlParser.h"
#include "StringUtils.h"
#include "Test.h"

using namespace std;

int main(int argc, char const *argv[]) {
    using namespace Test;

    DatabaseContext ctx;

    testInitContext(ctx);

    testCreateStudentTable(ctx);

    // testBiOperator();

    printf("Over!\n");
    return 0;
}