#include "TupleData.h"
#include "Assert.h"
#include "MallocUtils.h"

size_t TupleData::getTotalSize() const {
    size_t totalSize = sizeof(size_t) * length;
    for (size_t i = 0; i < length; ++i)
        totalSize += dataSize[i];
    return totalSize;
}

void TupleData::retireSpaceAndData() const {
    for (size_t i = 0; i < length; ++i)
        free(data[i]);
    free(data);
    free(dataSize);
}

void TupleData::printFormat(Column **columns, int columnSize) const {
    Assert::isTrue(columnSize == this->length, "not match!");
    printf("(");
    for (int i = 0; i < columnSize; i++) {
        switch (columns[i]->type) {
            case INT:
                printf("[I]%d", *static_cast<int *>(this->data[i]));
                break;
            case CHAR:
                printf("[C]%c", *static_cast<char *>(this->data[i]));
                break;
            case VARCHAR:
                printf("[S]%s", static_cast<char *>(this->data[i]));
                break;
        }
        if (i != columnSize - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

TupleData::TupleData(int tupleSize) : length(tupleSize) {
    this->data = static_cast<void **>(MallocUtils::mallocAndInitial(tupleSize * sizeof(void *)));
    this->dataSize = static_cast<size_t *>(MallocUtils::mallocAndInitial(tupleSize * sizeof(size_t)));
}

void TupleData::retireSpace() const {
    free(this->data);
    free(this->dataSize);
}

void TupleData::printFormat(const std::vector<ColumnPtr> &columns) const {
    Assert::isTrue(columns.size() == this->length, "not match!");
    printf("(");
    for (int i = 0; i < columns.size(); i++) {
        switch (columns[i]->type) {
            case INT:
                printf("[I]%d", *static_cast<int *>(this->data[i]));
                break;
            case CHAR:
                printf("[C]%c", *static_cast<char *>(this->data[i]));
                break;
            case VARCHAR:
                printf("[S]%s", static_cast<char *>(this->data[i]));
                break;
        }
        if (i != columns.size() - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

TupleData::TupleData() = default;

TupleData::Builder &TupleData::Builder::addString(const std::string &str) {
    order.push_back(ColumnTypeEnum::VARCHAR);
    strs.push_back(str);
    return *this;
}

TupleData::Builder &TupleData::Builder::addInt(int i) {
    order.push_back(ColumnTypeEnum::INT);
    ints.push_back(i);
    return *this;
}

TupleData::Builder &TupleData::Builder::addChar(char ch) {
    order.push_back(ColumnTypeEnum::CHAR);
    chars.push_back(ch);
    return *this;
}

#define MALLOC_AND_COPY(_size, _src, _target, _s) \
    _target = malloc(_size);                      \
    memcpy(_target, _src, _size);                 \
    _s = _size

TupleDataPtr TupleData::Builder::build() {
    size_t size = strs.size() + ints.size() + chars.size();

    auto pint = ints.begin();
    auto pchar = chars.begin();
    auto pstr = strs.begin();

    auto tuple = new TupleData{};
    tuple->data = (void **) malloc(size * sizeof(void *));
    tuple->dataSize = (size_t *) malloc(size * sizeof(size_t));
    tuple->length = size;

    size_t s;
    for (size_t i = 0; i < size; i++) {
        int iv;
        char cv;
        std::string sv;
        switch (order[i]) {
            case ColumnTypeEnum::INT:
                iv = *pint++;
                MALLOC_AND_COPY(sizeof(int), &iv, tuple->data[i],
                                tuple->dataSize[i]);
                break;
            case ColumnTypeEnum::CHAR:
                cv = *pchar++;
                MALLOC_AND_COPY(sizeof(char), &cv, tuple->data[i],
                                tuple->dataSize[i]);
                break;
            case ColumnTypeEnum::VARCHAR:
                sv = *pstr++;
                MALLOC_AND_COPY(sv.size() * sizeof(char) + 1, sv.c_str(),
                                tuple->data[i], tuple->dataSize[i]);
                break;
        }
    }
    return tuple;
}

#undef MALLOC_AND_COPY