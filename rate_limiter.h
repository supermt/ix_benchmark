//
// Created by jinghuan on 12/25/20.
//

#ifndef IX_BENCHMARK_RATE_LIMITER_H
#define IX_BENCHMARK_RATE_LIMITER_H

#include <mutex>
#include <atomic>

struct ms_clock {
    using rep = std::chrono::milliseconds::rep;
    using period = std::chrono::milliseconds::period;
    using duration = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<ms_clock, duration>;

    static
    time_point now() noexcept {
        return time_point(std::chrono::duration_cast<duration>(
                std::chrono::steady_clock::now().time_since_epoch()));
    }
};

class RateLimiter {
public:
    using clock = ms_clock;  // 1.


    const uint64_t limit_;
    const clock::duration interval_;
    const clock::rep interval_count_;

    mutable std::atomic<uint64_t>   count_{std::numeric_limits<uint64_t>::max()};  // 1.a.
    mutable std::atomic<clock::rep> timestamp_{0};                                 // 1.b.


public:
    constexpr RateLimiter(uint64_t limit, clock::duration interval) :
            limit_(limit), interval_(interval), interval_count_(interval_.count()) {}

    RateLimiter(const RateLimiter &) = delete;             // 3.a.
    RateLimiter &operator=(const RateLimiter &) = delete;  // 3.b.
    RateLimiter(RateLimiter &&) = delete;                  // 3.c.
    RateLimiter &operator=(RateLimiter &&) = delete;       // 3.d.

    bool reqeust() const;

    double get_qps() const {
        return 1000.0 * this->limit_ / this->interval_count_;
    }
};
#endif //IX_BENCHMARK_RATE_LIMITER_H
