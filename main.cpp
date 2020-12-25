#include <iostream>
#include "include/rate_limiter.h"
#include <unistd.h>
#include "include/slice.h"
#include "include/format.h"
#include "op_bucket.h"
#include <pthread.h>
#include <vector>
#include "Generator/key_gen.h"

std::vector<IX_NAME_SPACE::RequestEntry> key_array;

auto time_window_size = IX_NAME_SPACE::ms_clock::duration(1000);

namespace IX_NAME_SPACE {
    struct InserterArgs {
        InserterArgs(int qps, int num) : _qps(qps), _num(num) {}

        int _qps;
        int _num;
    };

    InserterArgs parse_int_args(void *arg) {
        InserterArgs result = *((InserterArgs *) arg);
        return result;
    }

    void fill_the_input_queue(std::vector<IX_NAME_SPACE::RequestEntry> *target_queue_ptr, int num, RateLimiter *limiter,
                              KeyGen *gen) {
        while (num > 0) {
            if (limiter->reqeust()) {
//                target_queue_ptr->push_back(gen->getNext());
                std::cout << gen->get_op() << " " << num << std::endl;
                num--;
            }
        }
        std::cout<< "end of running " << num <<std::endl;
        return;
    }

    void *read_inserter(void *arg) {
        InserterArgs parsed_args = parse_int_args(arg);
        int qps = parsed_args._qps;
        pthread_detach(pthread_self());
        std::cout << "read generator at QPS: " << qps << std::endl;
        RateLimiter read_limiter(qps, time_window_size);
        IX_NAME_SPACE::KeyGen generator;

        fill_the_input_queue(&key_array, parsed_args._num, &read_limiter, &generator);
//        delete (read_limiter);
        return NULL;
    }

    void *write_inserter(void *arg) {
        InserterArgs parsed_args = parse_int_args(arg);
        int qps = parsed_args._qps;
        pthread_detach(pthread_self());
        std::cout << "write generator at QPS: " << qps << std::endl;
        RateLimiter write_limiter(qps, time_window_size);
        IX_NAME_SPACE::KeyGen generator(index_benchmark::kWrite);

        fill_the_input_queue(&key_array, parsed_args._num, &write_limiter, &generator);
        return NULL;
//        delete (write_limiter);
    }
};


void fill_by_two_threads(int duration, int num) {
//    },pthread_t reader,pthread_t writer) {
    float read_speed = 100;
    float write_speed = 200;
    pthread_t reader, writer;
    IX_NAME_SPACE::InserterArgs reader_arg = IX_NAME_SPACE::InserterArgs(read_speed, num * (read_speed / (read_speed +
                                                                                                          write_speed)));
    IX_NAME_SPACE::InserterArgs writer_arg = IX_NAME_SPACE::InserterArgs(write_speed, num * (write_speed / (read_speed +
                                                                                                            write_speed)));

    int read_state = pthread_create(&reader, NULL, IX_NAME_SPACE::read_inserter, &reader_arg);
    int write_state = pthread_create(&writer, NULL, IX_NAME_SPACE::write_inserter, &writer_arg);

    std::cout << "poper started" << std::endl;

    sleep(duration);
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

//    sleep(duration);
//    pthread_cancel(reader);
//    pthread_cancel(writer);
    std::cout << "closed" << std::endl;
//    std::cout << key_array.size() << std::endl;
    return;
}

int main() {
    pthread_t read_thread_id;
    int a = 100;
    fill_by_two_threads(10, 1 * 1000);

    return 0;
}
