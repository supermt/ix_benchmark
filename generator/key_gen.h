//
// Created by jinghuan on 12/26/20.
//

#ifndef IX_BENCHMARK_KEY_GEN_H
#define IX_BENCHMARK_KEY_GEN_H

#pragma once

#include <stdlib.h>
#include "../include/format.h"
#include "../container/op_bucket.h"

namespace IX_NAME_SPACE {
    class KeyGen {
        // haven't figure it out yet.
        // TODO: add other generator hear, for now we use just the ... rand()
        // TO Shangyu: you can add some other generation here.
    private:
        OperationType _op;
    public:
        KeyGen() : _op(kQuery) {};

        KeyGen(OperationType op) : _op(op) {};

        RequestEntry getNext() {
            srand(time(NULL));
            return RequestEntry(_op, (double) rand());
        }

//        RequestEntry getNext(OperationType op) {
//            srand(time(NULL));
//            return RequestEntry(op, (double) rand());
//        }

        OperationType get_op() { return _op; }
    };

}


#endif //IX_BENCHMARK_KEY_GEN_H
