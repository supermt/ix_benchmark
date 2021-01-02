//
// Created by jinghuan on 12/25/20.
//

#ifndef IX_BENCHMARK_OP_BUCKET_H
#define IX_BENCHMARK_OP_BUCKET_H

#include "include/format.h"
#include "include/slice.h"

namespace IX_NAME_SPACE {
    class Slice;

//    class RequestEntry {
//        OperationType _op;
//        Key _key;
//        Slice _value; // allow to be empty
//        Key _endKey;// allow to be empty
//
//    public:
//        RequestEntry(OperationType op, double key) : _op(op), _key(key), _value("12345678"), _endKey(0.0) {}
//
//        RequestEntry() : _op(kQuery), _key(0.0), _value("12345678"), _endKey(0.0) {}
//
//        RequestEntry(OperationType op, Key key) : _op(op), _key(key), _value("12345678"), _endKey(0.0) {}
//
//        RequestEntry(OperationType op, Key key, const char *value) : _op(op), _key(key), _value(value), _endKey(0.0) {}
//
//        RequestEntry(OperationType op, double key, const char *value) : _op(op), _key(key), _value(value),
//                                                                        _endKey(0.0) {}
//
//        RequestEntry(OperationType op, double key, long long value) : _op(op), _key(key), _endKey(0.0) {
//            char str[9];
//            sprintf(str, "%lld", value);
//            _value = Slice(str);
//        }
//
//        RequestEntry(OperationType op, Key key, Slice value) : _op(op), _key(key), _value(value), _endKey(0.0) {}
//
//        RequestEntry(const RequestEntry &x) : _op(x._op), _key(x._key), _value(Slice(x._value)), _endKey(x._endKey) {}
//    };
    class RequestEntry {
    public:
        OperationType _op;
        double _key;
//        Slice _value; // allow to be empty
//        double _endKey = 0;// allow to be empty

    public:
        RequestEntry() : _op(kQuery), _key(0) {}

        RequestEntry(double key) : _op(kQuery), _key(key) {}

        RequestEntry(OperationType type) : _op(type), _key(0) {}

        RequestEntry(OperationType type, double key) : _op(type), _key(key) {}

        RequestEntry(const RequestEntry &x) : _op(x._op), _key(x._key) {}

        ~RequestEntry(){}
    };

};
#endif //IX_BENCHMARK_OP_BUCKET_H
