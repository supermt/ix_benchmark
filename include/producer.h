//
// Created by jinghuan on 12/30/20.
//

#ifndef IX_BENCHMARK_PRODUCER_H
#define IX_BENCHMARK_PRODUCER_H
#pragma once

#include "rate_limiter.h"
#include <pthread.h>
#include "generator/key_gen.h"
#include "container/concurrentqueue.h"
#include <vector>
#include <algorithm>

namespace IX_NAME_SPACE {
    // default time window with duration of one second
    static auto default_time_window_size = ms_clock::duration(1000);

//    struct InserterArgs {
//        InserterArgs(int qps, int num) : _qps(qps), _num(num),
//                                         _timewindow(default_time_window_size) {}
//
//        InserterArgs(int qps, int num, int time_window_millionsec) : _qps(qps), _num(num),
//                                                                     _timewindow(time_window_millionsec) {}
//
//        int _qps;
//        int _num;
//        ms_clock::duration _timewindow;
//        pthread_t worker_id;
//        bool depathed_or_not;
//        KeyGen* _gen;
//    };

    class reader;

    class writer;

    class producer;

    class scenario;

    class producer {
    public:
        long _num;
        KeyGen _gen;
        pthread_t worker_id;
        const bool depathed_or_not = true; // just for cases
        moodycamel::ConcurrentQueue<RequestEntry> *target_array_ptr;
        RateLimiter _limiter;
    public:
        producer(int duration, int num, float qps, moodycamel::ConcurrentQueue<RequestEntry> &key_array)
                : _limiter(qps, default_time_window_size) {
            target_array_ptr = &key_array;
            _num = std::min(static_cast<int>(duration * qps), num);
            _gen = KeyGen();
        }

        void setKeyGen(KeyGen keyGen) { _gen = KeyGen(keyGen); };

        static producer *parsed_from_voidptr(void *arg);

        virtual pthread_t create_inserter() = 0;

        void fill_the_queue();
    };

    class reader : producer {
    private:
//        InserterArgs args;
    public:
        reader(int duration,
               int num, float qps,
               moodycamel::ConcurrentQueue<RequestEntry> &key_array) :
                producer(duration, num, qps, key_array) {
            _gen = KeyGen(kQuery);
        }

        pthread_t create_inserter() override;

        static void *reader_threads(void *args);
    };

    class writer : producer {
    private:
//        InserterArgs args;
    public:
        writer(int duration,
               int num, float qps,
               moodycamel::ConcurrentQueue<RequestEntry> &key_array) :
                producer(duration, num, qps, key_array) {
            _gen = KeyGen(kWrite);
        }

        pthread_t create_inserter() override;

        static void *writer_threads(void *args);
    };

    class scenario {
        IX_NAME_SPACE::ms_clock::duration duration_seconds;

        scenario() : duration_seconds(10 * 1000) {};
    };

    class workload_engine {
        std::vector<reader> read_op_inserters;
        std::vector<writer> write_op_inserters;

        workload_engine() {
            read_op_inserters = std::vector<reader>();
            write_op_inserters = std::vector<writer>();
        }

        ~workload_engine() {
            read_op_inserters.clear();
            write_op_inserters.clear();
        }

    };
};


#endif //IX_BENCHMARK_PRODUCER_H
