#include "Table.h"
#include "Assert.h"
#include "ByteReader.h"
#include "ByteWriter.h"
#include "StringUtils.h"
#include "FileUtils.h"

#pragma region implement Table

const int Table::PG_CLASS_ID = 1259;
const int Table::PG_ATTRIBUTE_ID = 1249;
int Table::TABLE_ID_COUNT = 0;

Table::Table() = default;

Table::Table(const std::string &tablePath) : tablePath(tablePath) {
    ByteReader br(tablePath);
    // 读取对象的大小
    br.readULong();
    // 读取表的名称
    name = br.readString();
    // 读取表的列的个数
    size = br.readInt();

    // 初始化列空间
    columns =
            (ColumnPtr *) MallocUtils::mallocAndInitial(size * sizeof(ColumnPtr));

    // 初始化所有列
    for (size_t i = 0; i < size; i++) {
        auto col = new Column{};
        // 读取列名
        col->name = br.readCString();
        // 读取列的类型
        col->type = (ColumnTypeEnum) br.readInt();
        // 读取列的大小
        col->size = br.readULong();
        // 读取列的排序
        col->order = br.readInt();
        // 构造 Map
        columnMap[std::string(col->name)] = col;
        // 存储到列数组
        columns[i] = col;
    }
}

void Table::saveTableStruct(const std::string &path) {
    ByteWriter bw(path);
    bw.writeObject((Serializable *) this);
    bw.close();
}

byte_array Table::serialize() const {
    ByteWriter bw;
    // 先写入名称
    bw.writeString(name);
    // 再写入大小
    bw.writeInt((int) size);
    // 再遍历所有列
    for (size_t i = 0; i < size; i++) {
        // 写入列名
        bw.writeString(columns[i]->name);
        // 写入列的类型
        bw.writeInt(columns[i]->type);
        // 写入列的大小
        bw.writeULong(columns[i]->size);
        // 写入列的排序
        bw.writeInt(columns[i]->order);
    }
    // 返回写入的内容
    return bw.getBuffer();
}

Column *Table::operator[](const std::string &columnName) {
    return columnMap[columnName];
}

Column *Table::operator[](const char *columnName) {
    return columnMap[std::string(columnName)];
}

std::string Table::getName() {
    return this->name;
}

std::vector<char *> Table::getColumnNames() {
    std::vector<char *> result;

    for (int i = 0; i < size; i++)
        result.push_back(columns[i]->name);

    return result;
}

void Table::summary() {
    std::vector<char *> columnNames = this->getColumnNames();

    printf("|====================== Table[ %s ] PageSize[%ld] ============|\n",
           this->getName().c_str(), pages.size());
    printf(
            "|--------Name--------|--------Type--------|--------Size--------|\n");

    for (auto &columnName: columnNames) {
        Column *column = this->operator[](columnName);
        printf("|%20s|%20s(%d)|%20s|\n", column->name,
               StringUtils::toString(column->type).c_str(), column->type,
               std::to_string(column->size).c_str());
    }
}

size_t Table::insert(const TupleData &tupleData) {
    Assert::isTrue(tupleData.length == size, "the _size of tuple is not match");
    // 校验所有字段是否符合要求
    // 校验规则，大小不能超过指定大小
    for (size_t i = 0; i < tupleData.length; i++)
        Assert::isTrue(tupleData.dataSize[i] <= columns[i]->size,
                       "the _size of item[" + std::to_string(i) +
                       "] is bigger then table column");

    // 添加到页中
    if (pages.empty() || !pages.back()->append(tupleData)) {
        auto page = new Page(Page::APPEND | Page::MODIFY);
        page->pageHeader.tableId = tableId;
        Assert::isTrue(page->append(tupleData), "append error");
        pages.push_back(page);
    }
    return 1;
}

PagePtr Table::getPage(size_t pageNum) {
    Assert::isTrue(pageNum >= 0 && pageNum < pages.size(), "out of range");
    return this->pages[pageNum];
}

void Table::saveOnePage(size_t pageIdx) {
    Assert::isTrue(pageIdx < pages.size(), "out of range");
    /**
     * 如果需要修改某一页中的内容，需要保证 [0, pageIdx]
     * 之间没有新产生页数，否则将 [newIdx, pageIdx - 1] 的所有页都更新一遍
     */
    size_t left = pageIdx;
    // 找到最左需要写入文件的页
    for (size_t i = 0; i < pageIdx; i++)
        if (pages[i]->hasStates(Page::APPEND)) {
            left = i;
            break;
        }
    saveRangePage(left, pageIdx);
}

void Table::saveRangePage(size_t lRange, size_t rRange) {
    Assert::isTrue(lRange <= rRange && lRange >= 0 && rRange < pages.size(),
                   "out of range");
    auto bw =
            new ByteWriter(pagePath, (rRange - lRange + 1) * Page::PAGE_SIZE);
    bw->autoRetire = false;
    // 文件的偏移量
    size_t offset = lRange * Page::PAGE_SIZE;
    // 写指针跑到写
    // bw.goPos(_offset);
    bw->goPos(offset);
    for (size_t i = lRange; i <= rRange; i++) {
        byte_array bytes = pages[i]->serialize();
        // bw.write(bytes);
        bw->write(bytes);
    }
    // bw.close();
    bw->close();
}

void Table::saveAllPage() {
    if (this->pages.empty()) return;
    this->saveRangePage(0, pages.size() - 1);
}

TupleData Table::getRecordFromFile(size_t pageNum, size_t count) {
    ByteReader br(pagePath);
//    std::cout << "Read PagePath = " << pagePath << std::endl;
    size_t pageOffset =
            pageNum * Page::PAGE_SIZE + PageHeaderData::PAGE_HEADER_DATA_SIZE;
    size_t itemOffset = Item::ITEM_SIZE * count;
    br.goPos(pageOffset + itemOffset);

    // 读出 item 的 _offset 和 _size
    int _offset = br.readInt();
    int _size = br.readInt();

//    std::cout << "_offset = " << _offset << " _size = " << _size << std::endl;


    br.goPos(pageOffset + _offset);

    TupleData record{};
    record.dataSize =
            (size_t *) MallocUtils::mallocAndInitial(sizeof(size_t) * this->size);
    record.data = (void **) MallocUtils::mallocAndInitial(sizeof(void *) * this->size);
    record.length = this->size;

    for (size_t i = 0; i < this->size; i++) {
        switch (columns[i]->type) {
            case ColumnTypeEnum::VARCHAR:
                record.data[i] = br.readCString(record.dataSize[i]);
                break;
            case ColumnTypeEnum::INT:
                record.data[i] = new int{br.readInt()};
                record.dataSize[i] = columns[i]->size;
                break;
            case ColumnTypeEnum::CHAR:
                record.data[i] = new char{br.readChar()};
                record.dataSize[i] = columns[i]->size;
                break;
        }
    }

    return record;
}

std::vector<PagePtr> &Table::getPages() {
    return this->pages;
}

void Table::print(const TupleData &tuple) {
    printf("(");
    for (size_t i = 0; i < size; i++) {
        if (i != 0) printf(",");
        switch (columns[i]->type) {
            case ColumnTypeEnum::VARCHAR:
                printf("%s=", columns[i]->name);
                for (size_t j = 0; j < tuple.dataSize[i]; j++)
                    printf("%c", *((char *) tuple.data[i] + j));
                break;
            case ColumnTypeEnum::INT:
                printf("%s=%d", columns[i]->name, *(int *) tuple.data[i]);
                break;
            case ColumnTypeEnum::CHAR:
                printf("%s=%c", columns[i]->name, *(char *) tuple.data[i]);
                break;
        }
    }
    printf(")\n");
}

int32_t Table::getTableId() const {
    return tableId;
}

void Table::insertAttrs(TablePtr attrTable, TablePtr table) {
    // ==== pg_attribute ====
    // table_id     int
    // name         varchar
    // type         int
    // _size         int
    // order        int
    // nullable     int
    for (int i = 0; i < table->size; i++) {
        ColumnPtr column = table->columns[i];
        TupleDataPtr tuple = TupleData::Builder()
                .addInt(table->getTableId())
                .addInt(column->type)
                .addInt(column->size)
                .addInt(column->order)
                .addInt(column->nullable)
                .addString(column->name)
                .build();
        // 插入
        attrTable->insert(*tuple);
    }

    attrTable->saveAllPage();
}

void Table::insertTable(TablePtr classTable, TablePtr table) {
    // ==== pg_class ====
    // id     int
    // name   varchar
    // attr_count   int
//    printf("Table{id=%d, name=%s, size=%d}\n", table->getTableId(), table->getName().c_str(), table->getColumnSize());
    TupleDataPtr tuple = TupleData::Builder()
            .addInt(table->getTableId())
            .addInt(table->getColumnSize())
            .addString(table->getName())
            .build();

    classTable->insert(*tuple);

    classTable->saveAllPage();
}

TablePtr Table::loadClass(const std::string &classPath) {
    TablePtr classTable = buildClassTable(classPath);

//    printf("PageSize: %d\n", classTable->getPageSize());
    // 加载所有的表信息
    for (int i = 0; i < classTable->getPageSize(); i++)
        classTable->loadPage(i);
    // 返回
    return classTable;
}

TablePtr Table::loadAttribute(const std::string &attributePath) {
    TablePtr attributeTable = buildAttributeTable(attributePath);
    for (int i = 0; i < attributeTable->getPageSize(); i++)
        attributeTable->loadPage(i);
    return attributeTable;
}

TablePtr Table::buildClassTable(const std::string &pagePath) {
    // ==== pg_class ====
    // id     int
    // name   varchar
    // attr_count   int
    return Table::Builder()
            .tableId(Table::PG_CLASS_ID)
            .name("pg_class")
            .pagePath(pagePath)
            .addColumn("id", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("attr_count", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("name", ColumnTypeEnum::VARCHAR, 50)
            .build();
}

TablePtr Table::buildAttributeTable(const std::string &pagePath) {
    // ==== pg_attribute ====
    // table_id     int
    // name         varchar
    // type         int
    // size         int
    // order        int
    // nullable     int
    return Table::Builder()
            .tableId(Table::PG_ATTRIBUTE_ID)
            .name("pg_attribute")
            .pagePath(pagePath)
            .addColumn("table_id", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("type", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("size", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("order", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("nullable", ColumnTypeEnum::INT, sizeof(int))
            .addColumn("name", ColumnTypeEnum::VARCHAR, 50)
            .build();
}

int Table::getPageSize() const {
    return FileUtils::size(pagePath) / Page::PAGE_SIZE;
}

void Table::loadPage(int pageNum) {
    if (this->pages.size() > pageNum) return;
    int lRange = (int) this->pages.size();
    this->loadPage(lRange, pageNum);
}

void Table::loadPage(int lRange, int rRange) {
    if (this->pages.size() > rRange) return;
    lRange = (int) this->pages.size();

    FILE *f = fopen(pagePath.c_str(), "rb");

    byte_array tmp = MallocUtils::mallocBytes(Page::PAGE_SIZE);
    for (int i = lRange; i <= rRange; i++) {
        fseek(f, 0, i * Page::PAGE_SIZE);
        fread(tmp.value, 1, Page::PAGE_SIZE, f);
        auto page = new Page(tmp, columns, size);
        pages.push_back(page);
    }
}

ColumnPtr *Table::getColumns() const {
    return columns;
}

int Table::getColumnSize() const {
    return size;
}

#pragma endregion

#pragma region implement Table::Builder

Table::Builder::Builder() : product(new Table) {}

Table::Builder &Table::Builder::name(const std::string &name) {
    this->product->name = name;
    return *this;
}

Table::Builder &Table::Builder::addColumn(const std::string &columnName,
                                          ColumnTypeEnum type,
                                          int columnSize) {
    auto column = new Column{MallocUtils::cString(columnName), type, columnSize};

    this->columns.push_back(column);
    this->product->columnMap[columnName] = column;

    return *this;
}

Table::Builder &Table::Builder::pagePath(const std::string &path) {
    this->product->pagePath = path;
    return *this;
}

Table *Table::Builder::build() {
    if (this->columns.empty()) return nullptr;
    auto *columnArray =
            MALLOC_SIZE_OF_TYPE(this->columns.size(), ColumnPtr);

    int columnSize = (int) columns.size();

    for (int i = 0; i < columnSize; i++)
        columnArray[i] = columns[i], columnArray[i]->order = i;

    this->product->columns = columnArray;
    this->product->size = columnSize;
    this->product->tableId = this->id;
    return this->product;
}

Table::Builder &Table::Builder::tableId(int32_t tableId) {
    this->id = tableId;
    return *this;
}

TablePtr Table::create(const std::string &tableName, std::initializer_list<Column> columns) {
    Table::Builder builder;

    builder
            .tableId(Table::TABLE_ID_COUNT++)
            .name(tableName);

    for (const auto &column: columns) {
        builder.addColumn(column.name, column.type, column.size);
    }

    return builder.build();
}

void Table::setPagePath(const std::string &path) {
    Table::pagePath = path;
}

#pragma endregion
