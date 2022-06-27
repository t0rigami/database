//
// Created by Origami on 2022/6/27.
//

#ifndef DB_SERVER_RESULT_SET_H
#define DB_SERVER_RESULT_SET_H

#include <iostream>
#include <iterator>
#include "Column.h"
#include "TupleData.h"

class ResultSet {
public:
    class Iter {
    public:
        Iter(int p, ResultSet &target);

        bool hasNext();

        TupleDataPtr next();

        ~Iter();

    private:
        int p;
        ResultSet &target;
        TupleDataPtr v;
    };

    void append(TupleData tuple);

    const std::vector<TupleData> &getRawResult() const;

    Iter iter();

    const std::vector<ColumnPtr> &getMapping() const;

    void setMapping(ColumnPtr *columns, int size);

    void setMapping(const std::vector<ColumnPtr> &mapping);

    void appendMapping(ColumnPtr column);

private:
    std::vector<ColumnPtr> mapping;
    std::vector<TupleData> result;
};

#endif //DB_SERVER_RESULT_SET_H
