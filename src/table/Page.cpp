#include "Page.h"
#include "Assert.h"
#include "MallocUtils.h"
#include "BufferReader.h"

Page::Page(int32_t pageState) : state(pageState) {
    pageHeader.special = Page::PAGE_SIZE - Page::SPECIAL_SIZE;
    pageHeader.start = PageHeaderData::PAGE_HEADER_DATA_SIZE;
    pageHeader.lower = pageHeader.start;
    pageHeader.upper = pageHeader.special;
}

size_t Page::getTotalSize() const {
    // 总大小 = PAGE_SIZE - (upper - lower)
    return Page::PAGE_SIZE - pageHeader.upper + pageHeader.lower;
}

bool Page::isRemain(size_t size) const {
    return this->getTotalSize() + size <= Page::PAGE_SIZE;
}

bool Page::isRemain(TupleData tuple) const {
    return isRemain(tuple.getTotalSize() + Item::ITEM_SIZE);
}

bool Page::append(TupleData tupleData) {
    if (!isRemain(tupleData)) return false;
    this->tuples.push_back(tupleData);
    pageHeader.lower += Item::ITEM_SIZE;
    pageHeader.upper -= (int) tupleData.getTotalSize();
    return true;
}

void Page::remove(size_t index) {
    this->tuples.erase(tuples.begin() + (int) index);
}

void Page::removeAndRetire(size_t index) {
    auto &tuple = tuples[index];
    tuple.retireSpaceAndData();
    remove(index);
}

bool Page::hasStates(int32_t states) const {
    return state & states;
}

void Page::writerTo(ByteWriter &bw) const {
    byte_array bytes = serialize();
    bw.writeBytes(bytes);
    MallocUtils::retire(bytes);
}

#define _WRITE_BASIC_VALUES(_val) bw.write((byte*)&_val, sizeof(_val))
#define _WRITE_REF_VALUES(_src, _size) bw.write((byte*)_src, _size)

byte_array Page::serialize() const {
    ByteWriter bw(Page::PAGE_SIZE);
    // PageHeader
    _WRITE_BASIC_VALUES(pageHeader.tableId);
    _WRITE_BASIC_VALUES(pageHeader.start);
    _WRITE_BASIC_VALUES(pageHeader.lower);
    _WRITE_BASIC_VALUES(pageHeader.upper);
    _WRITE_BASIC_VALUES(pageHeader.special);
    // Items
    size_t offset = Page::PAGE_SIZE - Page::SPECIAL_SIZE;
    if (!tuples.empty()) {
        for (const auto &tuple: tuples) {
            size_t tupleSize = tuple.getTotalSize();
            offset -= tupleSize;
            _WRITE_BASIC_VALUES(offset);
            _WRITE_BASIC_VALUES(tupleSize);
//            printf("Item[%d]{offset=%ld, size=%ld}\n", offset, tupleSize);
        }
        // 将 bw 的 buffer _offset 移动到 _offset
        bw.setOffset(offset);
//        printf("Set Offset to %ld\n", offset);
        // TupleData s
        for (auto iter = tuples.rbegin(); iter != tuples.rend(); iter++) {
//            printf("Start tuples[%d] offset = %ld\n", j, bw.getOffset());
            // 遍历 tuple
            const TupleData &tuple = *iter;
            // 先写入数据大小，再写入数据内容
            for (size_t i = 0; i < tuple.length; i++) {
//                printf("dataSize[%d]=%ld\n", i, tuple.dataSize[i]);
                _WRITE_BASIC_VALUES(tuple.dataSize[i]);
                _WRITE_REF_VALUES(tuple.data[i], tuple.dataSize[i]);
            }
//            printf("End tuples[%d] offset = %ld\n", j--, bw.getOffset());
        }
    }
    // 将写指针移动到末尾
    bw.setOffsetEnd();
    // printf("serialize over !!!.. return\n");
    return bw.getBuffer();
}

Page::Page(const byte_array &bytes, ColumnPtr *columns, int size) {
    auto pageHeaderDataPtr = (PageHeaderDataPtr) bytes.value;
    this->pageHeader.lower = pageHeaderDataPtr->lower;
    this->pageHeader.upper = pageHeaderDataPtr->upper;
    this->pageHeader.special = pageHeaderDataPtr->special;
    this->pageHeader.tableId = pageHeaderDataPtr->tableId;
    this->pageHeader.start = pageHeaderDataPtr->start;

//    printf("lower = %d, upper = %d, special = %d, table_id = %ld, start = %d\n", pageHeader.lower, pageHeader.upper,
//           pageHeader.special, pageHeader.tableId, pageHeader.start);

    int offset = pageHeaderDataPtr->start;
    int count = (pageHeaderDataPtr->lower - pageHeaderDataPtr->start) / (int) Item::ITEM_SIZE;

//    std::cout << "count = " << count << std::endl;

    for (int i = 0; i < count; i++) {
        auto item = (ItemPtr) (bytes.value + offset);
//        printf("Item=%d {offset=%zu, size=%zu}\n", offset, item->offset, item->size);
        byte_array b{bytes.value + item->offset, static_cast<size_t>(item->size)};
        BufferReader reader(b);

//        printf("Item Size = %ld\n", b.size);

        TupleData::Builder builder;

        for (int j = 0; j < size; j++) {
//            printf("start offset=%ld\n", reader.getOffset());
            switch (columns[j]->type) {
                case INT: {
                    reader.readULong();
//                    printf("read dataSize[%d]=%ld\n", j, dataSize);
                    int num = reader.readInt();
//                    printf("num = %d ", num);
                    builder.addInt(num);
                }
                    break;
                case CHAR: {
                    reader.readULong();
//                    printf("read dataSize[%d]=%ld\n", j, dataSize);
                    char ch = reader.readChar();
                    printf("ch = %c ", ch);
                    builder.addChar(ch);
                }
                    break;
                case VARCHAR: {
                    std::string s = reader.readString();
//                    printf("s = %s ", s.c_str());
                    builder.addString(s);
                }
                    break;
                default:
                    Assert::isTrue(false, "not match this type!");
            }
//            printf("\nend offset=%ld\n", reader.getOffset());
//            puts("======>");
        }

        this->tuples.push_back(*builder.build());
        offset += sizeof(Item);
    }
}

const std::vector<TupleData> &Page::getTuples() const {
    return tuples;
}

int Page::getCount() {
    return (int) this->tuples.size();
}

#undef _WRITE_BASIC_VALUES
#undef _WRITE_REF_VALUES