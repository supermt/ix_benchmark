#include <iostream>
#include "include/rate_limiter.h"
#include <unistd.h>
#include "include/slice.h"
#include "include/format.h"
#include "op_bucket.h"

int main() {
    auto time_window_size = ms_clock::duration(1000);


    RateLimiter *read_limiter = new RateLimiter(10000, time_window_size);
    RateLimiter write_limiter(1000, time_window_size);
    std::cout << read_limiter->get_qps() << std::endl;
    std::cout << write_limiter.get_qps() << std::endl;
    int i = 0;
    auto start = std::chrono::system_clock::now();
    while (i < 1 * 10 * 1000) {
        if (i > 20000 && read_limiter->limit_ != 1000) {
            read_limiter = new RateLimiter(1000, time_window_size);
        }
        if (read_limiter->reqeust()) {
            auto elapsed = std::chrono::system_clock::now() - start;
            if (i % 100 == 0) {
                std::cout << "processed:" << i << " "
                          << static_cast<double>(i) /
                             (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() +
                              1) << " req./s" << std::endl;
            }
            i++;
        }
    }

    return 0;
}
