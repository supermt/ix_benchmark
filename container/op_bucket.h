//
// Created by jinghuan on 12/25/20.
//

#ifndef IX_BENCHMARK_OP_BUCKET_H
#define IX_BENCHMARK_OP_BUCKET_H

#include "../include/format.h"
#include "../include/slice.h"

namespace IX_NAME_SPACE {
    template<typename KeyType, typename ValueType>
    class RequestEntry {
    public:
        OperationType _op;
        KeyType _key;
        ValueType _value;

    public:
        RequestEntry() : _op(kQuery), _key(0), _value(0) {}

        RequestEntry(KeyType key, ValueType value) 
                        : _op(kQuery), _key(key), _value(value) {}

        RequestEntry(OperationType type) : _op(type), _key(0), _value(0) {}

        RequestEntry(OperationType type, KeyType key, ValueType value) 
                        : _op(type), _key(key), _value(value) {}

        RequestEntry(const RequestEntry &x) 
                        : _op(x._op), _key(x._key), _value(x._value) {}
    };

};
#endif //IX_BENCHMARK_OP_BUCKET_H
