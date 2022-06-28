//
// Created by Origami on 2022/6/27.
//
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
#include "TupleData.h"
#include "Sort.h"
#include <algorithm>
#include <random>
#include "FixedReader.h"

using namespace std;

namespace Test {
    const char *STUDENT_TABLE_NAME = "student_7320";
    const int STUDENT_TABLE_ID = 3;
    const char *TEACHER_TABLE_NAME = "teacher";
    const int TEACHER_TABLE_ID = 2;


    __attribute__((unused)) void testInitContext(DatabaseContext &ctx) {
        // ctx.initConfiguration("conf/db.conf");

        ctx.initSqlExecutor();

        ctx.initTable();

        // ctx.initServerSocket();

        // ctx.start();
    }


    __attribute__((unused))  void testCreateStudentTable(DatabaseContext &ctx) {
        TablePtr studentTable = Table::create(STUDENT_TABLE_NAME, STUDENT_TABLE_ID, {
                {(char *) "name", VARCHAR, 50, 0, false},
                {(char *) "age",  INT,     4,  1, false}
        });

        if (ctx.registerTable(studentTable, true)) {
            printf("register %s successful!\n", STUDENT_TABLE_NAME);
            return;
        }
        printf("register %s failed!\n", STUDENT_TABLE_NAME);
    }


    __attribute__((unused)) void testInsertToStudentTable(DatabaseContext &ctx) {
        TablePtr studentTable = ctx.getTableByName(STUDENT_TABLE_NAME);
        if (studentTable == nullptr) {
            printf("not find %s table\n", STUDENT_TABLE_NAME);
            return;
        }

        for (int i = 0; i < 400; i++) {
            studentTable->insert(*TupleData::Builder()
                    .addString("ZhangSan" + to_string(i))
                    .addInt(i)
                    .build());
        }

        for (int i = 0; i < studentTable->getPageSize(); ++i) {
            for (const auto &item: studentTable->getPage(i)->getTuples()) {
                studentTable->formatPrint(item);
            }
        }

        studentTable->saveAllPage();
    }


    __attribute__((unused)) void testPrintStudentTable(DatabaseContext &ctx) {
        TablePtr studentTable = ctx.getTableByName(STUDENT_TABLE_NAME);

        if (studentTable == nullptr) {
            printf("can not find table %s\n", STUDENT_TABLE_NAME);
            return;
        }

        printf("Table [%s]\n", studentTable->getName().c_str());
        studentTable->loadAllPage();

        for (const auto &page: studentTable->getPages()) {
            for (const auto &tuple: page->getTuples()) {
                studentTable->formatPrint(tuple);
            }
        }
    }


    __attribute__((unused)) void testCreateTeacherTable(DatabaseContext &ctx) {
        TablePtr teacherTable = Table::create(TEACHER_TABLE_NAME, TEACHER_TABLE_ID, {
                {(char *) "id",       INT,     50, 0, false},
                {(char *) "name",     VARCHAR, 50, 1, false},
                {(char *) "address",  VARCHAR, 50, 1, false},
                {(char *) "position", VARCHAR, 50, 1, false},
        });
        if (ctx.registerTable(teacherTable, true)) {
            printf("register %s successful!\n", TEACHER_TABLE_NAME);
            return;
        }
        printf("register %s failed!\n", TEACHER_TABLE_NAME);
    }


    __attribute__((unused)) void testInsertToTeacherTable(DatabaseContext &ctx) {
        TablePtr teacherTable = ctx.getTableByName(TEACHER_TABLE_NAME);
        if (teacherTable == nullptr) {
            printf("not find %s table\n", TEACHER_TABLE_NAME);
            return;
        }

        for (int i = 0; i < 400; i++) {
            teacherTable->insert(*TupleData::Builder()
                    .addInt(i)
                    .addString("LiSi" + to_string(i))
                    .addString("China" + to_string(400 - i))
                    .addString("Professor" + to_string(i))
                    .build());
        }

        teacherTable->saveAllPage();
    }


    __attribute__((unused)) void testPrintTeacherTable(DatabaseContext &ctx) {
        TablePtr teacherTable = ctx.getTableByName(TEACHER_TABLE_NAME);

        if (teacherTable == nullptr) {
            printf("can not find table %s\n", TEACHER_TABLE_NAME);
            return;
        }

        printf("Table [%s]\n", teacherTable->getName().c_str());
        teacherTable->loadAllPage();

        for (const auto &page: teacherTable->getPages()) {
            for (const auto &tuple: page->getTuples()) {
                teacherTable->formatPrint(tuple);
            }
        }
    }

    __attribute__((unused)) void testSelectSimple(DatabaseContext &ctx) {
        TablePtr pgAttribute = ctx.getTableByName("pg_attribute");

        printf("========= total record =======\n");

        for (int i = 0; i < pgAttribute->getPageSize(); ++i) {
            for (const auto &item: pgAttribute->getPage(i)->getTuples()) {
                pgAttribute->formatPrint(item);
            }
        }

        auto &executor = ctx.getSqlExecutor();

        printf("execute SQL: select * from pg_attribute where table_id = 1259\n");

        ResultSet resultSet =
                executor.execSimpleQuery("pg_attribute", "table_id = 1259", {});

        printf("Result:\n");

        for (const auto &item: resultSet.getRawResult()) {
            pgAttribute->formatPrint(item);
        }
    }


    __attribute__((unused)) void testSelectSimpleWithProjection(DatabaseContext &ctx) {
        TablePtr pgAttribute = ctx.getTableByName("pg_attribute");

        printf("========= total record =======\n");

        for (int i = 0; i < pgAttribute->getPageSize(); ++i) {
            for (const auto &item: pgAttribute->getPage(i)->getTuples()) {
                pgAttribute->formatPrint(item);
            }
        }

        auto &executor = ctx.getSqlExecutor();

        printf(
                "execute SQL: select table_id, name, size from pg_attribute where "
                "table_id = 1259\n");

        ResultSet resultSet = executor.execSimpleQuery(
                "pg_attribute", "table_id = 1259", {"table_id", "name", "size"});

        printf("Result:\n");
    }


    __attribute__((unused)) void testPrintAllTable(DatabaseContext &ctx) {
        for (const auto &table: ctx.getTableManager()->getTables()) {
            table.second->summary();
        }
    }


    __attribute__((unused)) void testBiOperator(const std::string &expression = "100 < age") {
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

    __attribute__((unused)) void testNumberQuickSort() {
        vector<int> v;
        for (int i = 0; i < 400; i++) v.push_back(i);
        shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));

        v = Sort::quickSort(v, [](int a, int b) {
            return a - b;
        });

        for_each(v.begin(), v.end(), [](int a) {
            cout << a << endl;
        });
    }

    TablePtr _prepareData(DatabaseContext &ctx, bool insert = true, int count = 400) {
        TablePtr studentTable = Table::create(STUDENT_TABLE_NAME, STUDENT_TABLE_ID, {
                {(char *) "name", VARCHAR, 50, 0, false},
                {(char *) "age",  INT,     4,  1, false}
        });


        studentTable->setPagePath(ctx.getDatabaseConfig().getPagePath() + "/" + std::to_string(STUDENT_TABLE_ID) +
                                  ".pg");


        if (insert) {
            for (int i = 0; i < count; i++) {
                studentTable->insert(*TupleData::Builder()
                        .addString("ZhangSan" + to_string(i))
                        .addInt(i)
                        .build());
            }
        }
        return studentTable;
    }

    __attribute__((unused)) void testQuickSort(DatabaseContext &ctx) {
        printf(">>>>>>>>>>>>> Start TestQuickSort <<<<<<<<<<<<<<<\n");
        TablePtr studentTable = _prepareData(ctx);
        PagePtr page = studentTable->getPage(0);


        auto record = page->getTuples();

        shuffle(record.begin(), record.end(), std::mt19937(std::random_device()()));

        for (const auto &item: record) {
            studentTable->formatPrint(item);
        }

        record = Sort::quickSort(record, [&](TupleData &a, TupleData &b) -> int {
            int ageA = studentTable->selectInt("age", a);
            int ageB = studentTable->selectInt("age", b);
            return ageA - ageB;
        });

        printf("<<<<<<<<<<<<<<<< Sort End >>>>>>>>>>>>>>>>\n");

        for (const auto &item: record) {
            studentTable->formatPrint(item);
        }
    }


    void testReadFixed(DatabaseContext &ctx) {
        TablePtr studentTable = _prepareData(ctx, false);

        FixedReader fr(512, "./tmp/0.tp");

        while (fr.hasNext()) {
            byte_array bytes = fr.readObjectByteArray();
            TupleData tupleData = TupleData::fromBytes(bytes);
            MallocUtils::retire(bytes);
            studentTable->formatPrint(tupleData);
        }
        fr.close();
    }


    __attribute__((unused)) void testExternalSort(DatabaseContext &ctx, bool insert = false) {
        TablePtr studentTable = nullptr;
        if (insert) {
            studentTable = _prepareData(ctx, true, 1);
        } else {
            studentTable = _prepareData(ctx, true, 0);
        }
        auto &tempPath = ctx.getDatabaseConfig().getTempPath();
        for (int i = 0; i < studentTable->getPageSize(); ++i) {
            for (const auto &item: studentTable->getPage(i)->getTuples()) {
                studentTable->formatPrint(item);
            }
        }
        Sort::externalSort(studentTable, "./sorted.tp", ctx.getDatabaseConfig().getTempPath(),
                           [&](TupleData &a, TupleData &b) -> int {
                               int ageA = studentTable->selectInt("age", a);
                               int ageB = studentTable->selectInt("age", b);
                               return ageA - ageB;
                           });
    }

}  // namespace Test