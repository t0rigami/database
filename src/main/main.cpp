#include <time.h>
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

using namespace std;

int main(int argc, char const *argv[]) {
    DatabaseContext context;
//
    context.initTable();

/*    // // 读取配置文件
    // context.initConfiguration("conf/db.conf");

    // // 初始化服务器 Socket
    // context.initServerSocket();

    // 启动监听
    // context.start();*/

    TablePtr student_7320_table = Table::create("student_7320",
                                                {
                                                        {"name", ColumnTypeEnum::VARCHAR, 50, 0, false},
                                                        {"age",  ColumnTypeEnum::INT,     4,  0, false}
                                                });

    Assert::isTrue(context.registerTable(student_7320_table), "register error!");

    for (int i = 0; i < 400; i++) {
        // 插入到表内
        student_7320_table->insert(*TupleData::Builder()
                .addString("zhangsan" + to_string(i))
                .addInt(i)
                .build());
    }

    student_7320_table->saveAllPage();

    for (const auto &item: context.getTables()) {
        item.second->summary();
    }

    printf("Over!\n");

    return 0;
}