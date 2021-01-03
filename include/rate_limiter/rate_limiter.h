//
// Created by jinghuan on 1/3/21.
//

#ifndef IX_BENCHMARK_RATE_LIMITER_H
#define IX_BENCHMARK_RATE_LIMITER_H

#include <mutex>

#pragma once

#include "include/rate_limiter/rate_limiter_interface.h"

namespace IX_NAME_SPACE {
    class RateLimiter : public RateLimiterInterface {
    public:
        RateLimiter();

        long aquire();

        long aquire(int permits);

        bool try_aquire(int timeouts);

        bool try_aquire(int permits, int timeout);

        double get_rate() const;

        void set_rate(double rate);

    private:
        void sync(unsigned long long now);

        std::chrono::microseconds claim_next(double permits);

    private:
        double interval_;
        double max_permits_;
        double stored_permits_;

        unsigned long long next_free_;

        std::mutex mut_;
    };
};


#endif //IX_BENCHMARK_RATE_LIMITER_H
