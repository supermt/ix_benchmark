//
// Created by jinghuan on 12/25/20.
//

#ifndef IX_BENCHMARK_FORMAT_H
#define IX_BENCHMARK_FORMAT_H

#include <cstdio>
#include <cstdlib>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

#include "ix_namespace.h"
#include "slice.h"

namespace IX_NAME_SPACE {
    enum OperationType {
        kQuery = 0x00, // point look up
        kWrite = 0x01, // insert new data
        kUpdate = 0x02,// update without check
        kUpdateWithHint = 0x03, // update, if exists
        kDelete = 0x04, // single delete
        kSeek = 0x05, // range query
    };
    enum KeyType {
        kDouble,
        kString
    };

    struct Key {
        KeyType _type;
        union {
            double value;
            Slice slice_value;
        };

        Key(double a) : _type(kDouble) { value = a; }

        Key(Slice slice) : _type(kString) { slice_value = slice; }

        Key(const char *input) : _type(kString) { slice_value = Slice(input); }
    };
};


#endif //IX_BENCHMARK_FORMAT_H
