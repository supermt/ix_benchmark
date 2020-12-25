//
// Created by jinghuan on 12/25/20.
//

#include "rate_limiter.h"


bool RateLimiter::reqeust() const{
    auto orig_count = this->count_.fetch_add(1UL, std::memory_order_acq_rel);
    if (orig_count < this->limit_) {
        return true;
    } else {
        auto ts  = this->timestamp_.load();
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