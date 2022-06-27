//
// Created by Origami on 2022/6/27.
//
#include "ResultSet.h"

void ResultSet::append(TupleData tuple) {
    this->result.push_back(tuple);
}

const std::vector<TupleData> &ResultSet::getRawResult() const {
    return result;
}

ResultSet::Iter ResultSet::iter() {
    return ResultSet::Iter{0, *this};
}

const std::vector<ColumnPtr> &ResultSet::getMapping() const {
    return mapping;
}

void ResultSet::setMapping(ColumnPtr *columns, int size) {
    this->mapping.clear();
    for (int i = 0; i < size; i++) {
        this->mapping.push_back(columns[i]);
    }
}

void ResultSet::setMapping(const std::vector<ColumnPtr> &mapping) {
    ResultSet::mapping = mapping;
}

void ResultSet::appendMapping(ColumnPtr column) {
    this->mapping.push_back(column);
}

ResultSet::Iter::Iter(int p, ResultSet &target) : p(p), target(target) {
    v = new TupleData((int) target.mapping.size());
}

bool ResultSet::Iter::hasNext() {
    return target.result.size() > p;
}

TupleDataPtr ResultSet::Iter::next() {
    for (int i = 0; i < target.mapping.size(); ++i) {
        v->data[i] = target.result[p].data[target.mapping[i]->order];
        v->dataSize[i] = target.result[p].dataSize[target.mapping[i]->order];
    }
    p++;
    return v;
}

ResultSet::Iter::~Iter() {
    v->retireSpace();
}
