#include <wait.h>
#include <ctime>
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
#include "SqlParser.h"
#include "StringUtils.h"
#include "Table.h"
#include "Test.h"
#include "TupleData.h"

using namespace std;

int main(int argc, char const *argv[]) {
    using namespace Test;

    DatabaseContext ctx;

//    testInitContext(ctx);

//    testCreateStudentTable(ctx);

    //    testCreateTeacherTable(ctx);

    // testPrintAllTable(ctx);

//    testInsertToStudentTable(ctx);

    // testInsertToTeacherTable(ctx);

//    testPrintStudentTable(ctx);

    // testPrintTeacherTable(ctx);

    // testBiOperator();

//    testNumberQuickSort();

    testQuickSort(ctx);


    return 0;
}
