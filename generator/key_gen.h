//
// Created by jinghuan on 12/26/20.
//

#ifndef IX_BENCHMARK_KEY_GEN_H
#define IX_BENCHMARK_KEY_GEN_H

#include <stdlib.h>
#include "../include/format.h"
#include "../include/container/op_bucket.h"

namespace IX_NAME_SPACE {
    template<typename KeyType, typename ValueType>
    class KeyGen {
    protected:
        OperationType _op;
        double _read_ratio;
    public:
        KeyGen() = delete;
        KeyGen(OperationType op) : _op(op) { }
        virtual RequestEntry<KeyType, ValueType> getNext() = 0;
        
        void set_read_ratio(double rr) {
            _read_ratio = rr;
        }
    };

    template<typename KeyType, typename ValueType>
    class LogNormalGen : public KeyGen<KeyType, ValueType> {
    private:
        std::lognormal_distribution<double> _dist;
        std::mt19937_64 _rand;
        const double _amp = 1e9;
    public:
        LogNormalGen(OperationType op) 
                    : KeyGen<KeyType, ValueType>(op), _dist(0., 1.), 
                        _rand(std::random_device{}()) { }

        LogNormalGen(OperationType op, double mean, double var) 
                    : KeyGen<KeyType, ValueType>(op), _dist(mean, var), 
                        _rand(std::random_device{}()) { }

        virtual RequestEntry<KeyType, ValueType> getNext() {
            KeyType key = static_cast<KeyType>(_dist(_rand) * _amp);
            ValueType value = 1;
            return RequestEntry<KeyType, ValueType>(KeyGen<KeyType, ValueType>::_op, key, value);
        }
    };
}


#endif //IX_BENCHMARK_KEY_GEN_H
