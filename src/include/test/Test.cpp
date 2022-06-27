//
// Created by Origami on 2022/6/27.
//
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

using namespace std;

namespace Test {
    const string STUDENT_TABLE_NAME = "student_7320";

    void initContext(DatabaseContext &ctx) {

        // ctx.initConfiguration("conf/db.conf");

        ctx.initSqlExecutor();

        ctx.initTable();

        // ctx.initServerSocket();

        // ctx.start();
    }

    void createStudentTable(DatabaseContext &ctx) {
        TablePtr studentTable = Table::create(STUDENT_TABLE_NAME, {
                {"name", ColumnTypeEnum::VARCHAR, 50, 0, false},
                {"age",  ColumnTypeEnum::INT,     4,  0, false}
        });

        if (ctx.registerTable(studentTable)) {
            printf("register %s successful!\n", STUDENT_TABLE_NAME.c_str());
        }
        printf("register %s failed!\n", STUDENT_TABLE_NAME.c_str());
    }

    void insertToStudentTable(DatabaseContext &ctx) {
        TablePtr studentTable = ctx.getTableByName(STUDENT_TABLE_NAME);
        if (studentTable == nullptr) {
            printf("not find %s table\n", STUDENT_TABLE_NAME.c_str());
            return;
        }

        for (int i = 0; i < 400; i++) {
            studentTable->insert(*TupleData::Builder()
                    .addString("ZhangSan" + to_string(i))
                    .addInt(i)
                    .build());
        }

        studentTable->saveAllPage();
    }

    void selectSimple(DatabaseContext &ctx) {
        TablePtr pgAttribute = ctx.getTableByName("pg_attribute");

        printf("========= total record =======\n");

        for (int i = 0; i < pgAttribute->getPageSize(); ++i) {
            for (const auto &item: pgAttribute->getPage(i)->getTuples()) {
                pgAttribute->print(item);
            }
        }

        auto &executor = ctx.getSqlExecutor();

        printf("execute SQL: select * from pg_attribute where table_id = 1259\n");

        ResultSet resultSet = executor.execSimpleQuery("pg_attribute", "table_id = 1259", {});

        printf("Result:\n");

        for (const auto &item: resultSet.getRawResult()) {
            pgAttribute->print(item);
        }
    }

    void selectSimpleWithProjection(DatabaseContext &ctx) {
        TablePtr pgAttribute = ctx.getTableByName("pg_attribute");

        printf("========= total record =======\n");

        for (int i = 0; i < pgAttribute->getPageSize(); ++i) {
            for (const auto &item: pgAttribute->getPage(i)->getTuples()) {
                pgAttribute->print(item);
            }
        }

        auto &executor = ctx.getSqlExecutor();

        printf("execute SQL: select table_id, name, size from pg_attribute where table_id = 1259\n");

        ResultSet resultSet = executor.execSimpleQuery("pg_attribute", "table_id = 1259",
                                                       {"table_id", "name", "size"});

        printf("Result:\n");

    }

    void testBiOperator(const std::string &expression = "100 < age") {
        printf("expression = %s\n", expression.c_str());
        Optional<BiRelation> optional = SqlParser::toBiRelation(expression);

        if (optional.empty()) {
            cout << "parser error!" << endl;
            return;
        }

        BiRelation bi = optional.get();

        printf("parser result:\n");
        bi.print();

        printf("swap expression:\n");
        bi.swapOperand();

        bi.print();
    }
}