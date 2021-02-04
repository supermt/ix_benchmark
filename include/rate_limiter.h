//
// Created by jinghuan on 12/25/20.
//

#ifndef IX_BENCHMARK_RATE_LIMITER_H
#define IX_BENCHMARK_RATE_LIMITER_H

#include <mutex>
#include <atomic>
#include "format.h"

namespace IX_NAME_SPACE {

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


        uint64_t limit_;
        const clock::duration interval_;
        const clock::rep interval_count_;

        mutable std::atomic<uint64_t> count_{std::numeric_limits<uint64_t>::max()};  // 1.a.
        mutable std::atomic<clock::rep> timestamp_{0};                                 // 1.b.


    public:
        constexpr RateLimiter(uint64_t limit, clock::duration interval) :
                limit_(limit), interval_(interval), interval_count_(interval_.count()) {}

        constexpr RateLimiter(clock::duration interval) :
                limit_(0), interval_(interval), interval_count_(interval_.count()) {}

        RateLimiter(const RateLimiter &) = delete;             // 3.a.
        RateLimiter &operator=(const RateLimiter &) = delete;  // 3.b.
        RateLimiter(RateLimiter &&) = delete;                  // 3.c.
        RateLimiter &operator=(RateLimiter &&) = delete;       // 3.d.

        bool request() const {
            auto orig_count = this->count_.fetch_add(1UL, std::memory_order_acq_rel);
            if (orig_count < this->limit_) {
                return true;
            } else {
                auto ts = this->timestamp_.load();
                auto now = clock::now().time_since_epoch().count();
                if (now - ts < this->interval_count_) {
                    return false;
                }
                if (not this->timestamp_.compare_exchange_strong(ts, now)) {
                    return false;
                }
                if (ts == 0) {
                    auto orig_count = this->count_.fetch_add(1UL, std::memory_order_acq_rel);
                    return (orig_count < this->limit_);
                }
                this->count_.store(1UL, std::memory_order_release);
                return true;
            }
        }

        void set_limit(uint64_t limit) { limit_ = limit; }

        double get_qps() const {
            return 1000.0 * this->limit_ / this->interval_count_;
        }
    };
};
#endif //IX_BENCHMARK_RATE_LIMITER_H
