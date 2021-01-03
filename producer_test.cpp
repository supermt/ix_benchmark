#include <iostream>
#include "include/rate_limiter.h"
#include <unistd.h>
#include "include/slice.h"
#include "include/format.h"
#include "include/container/op_bucket.h"
#include <pthread.h>
#include <vector>
#include "generator/key_gen.h"
#include <iomanip>
#include "include/container/concurrentqueue.h"
#include "include/producer.h"

moodycamel::ConcurrentQueue<IX_NAME_SPACE::RequestEntry> key_array;

auto time_window_size = IX_NAME_SPACE::ms_clock::duration(1000);

namespace IX_NAME_SPACE {

    void fill_by_two_threads(int duration, int num, float read_qps, float write_qps) {
//    },pthread_t Reader,pthread_t Writer) {
        float read_speed = read_qps;
        float write_speed = write_qps;

        Reader worker1(duration, num * (read_speed / (read_speed + write_speed)),
                       read_qps, key_array);
        Writer worker2(duration, num * (write_speed / (read_speed + write_speed)),
                       write_qps, key_array);
        pthread_t workerid1, workerid2;
        workerid1 = worker1.create_inserter();
        workerid2 = worker2.create_inserter();
//        std::cout << sizeof(worker1) << std::endl;

        std::cout << "poper started" << std::endl;

        IX_NAME_SPACE::RequestEntry temp;

        while (num > 0) {
            while (key_array.try_dequeue(temp)) {
                std::cout << "dequeue the entry: " << std::fixed << temp._key.to_string() << " entry seq: " << num
                          << std::endl;
                num--;
            };
        }

        return;
    }
}

int main() {
    pthread_t read_thread_id;
    int a = 100;
    IX_NAME_SPACE::fill_by_two_threads(10, 1 * 1000, 100, 200);

    // TO Shangyu:
    // The easiest way to generate a R/W shifting workload, for example, you can run the following functions for many times
//    fill_by_two_threads(10, 1 * 1000, 200, 200); // this is a R:W 50% to 50%
//    fill_by_two_threads(10, 10 * 1000, 800, 200); // this is a R:W 80% : 20%
//    fill_by_two_threads(10, 1000 * 1000, 200, 200); // this is a R:W 50% to 50% workload again
    // the duration and _num, as you can see in the definition of the function, if the system run out of _num first,
    // the worker threads will shot and the main thread will sleep, until it's awaken.
    // However, if the duration time is run out of first, it will shutdown the threads, so you don;t need to worry about the
    // total number of it.

    return 0;
}
