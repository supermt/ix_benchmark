#include <iostream>
#include "rate_limiter.h"
#include <unistd.h>

int main() {

    auto time_window_size = ms_clock::duration(1000);

    RateLimiter read_limiter(10000, time_window_size);
    RateLimiter write_limiter(1000, time_window_size);
    std::cout << read_limiter.get_qps() << std::endl;
    std::cout << write_limiter.get_qps() << std::endl;
    int i = 0;
    auto start = std::chrono::system_clock::now();
    while (i < 100 * 1000 * 1000) {
        if (read_limiter.reqeust()) {
            auto elapsed = std::chrono::system_clock::now() - start;
            if (i % 100 == 0) {
                std::cout << "processed:" << i << " "
                          << static_cast<double>(i) / (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() +
                                                       1) << " req./s" << std::endl;
            }
            i++;
        }
    }


    return 0;
}
