//
// Created by jinghuan on 1/3/21.
//

#ifndef IX_BENCHMARK_RATE_LIMITER_INTERFACE_H
#define IX_BENCHMARK_RATE_LIMITER_INTERFACE_H

#include "include/format.h"
#include "include/slice.h"

namespace IX_NAME_SPACE {
    class RateLimiterInterface {
    public:
        virtual ~RateLimiterInterface() {}

        virtual long aquire() = 0;

        virtual long aquire(int permits) = 0;

        virtual bool try_aquire(int timeout) = 0;

        virtual bool try_aquire(int permits, int timeout) = 0;

        virtual double get_rate() const = 0;

        virtual void set_rate(double rate) = 0;
    };
};


#endif //IX_BENCHMARK_RATE_LIMITER_INTERFACE_H
