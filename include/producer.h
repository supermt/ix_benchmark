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

    class Reader;

    class Writer;

    class Producer;

    class scenario;

    class Producer {
    public:
        long _num;
        KeyGen _gen;
        pthread_t worker_id = -1;
        const bool depathed_or_not = true; // just for cases
        moodycamel::ConcurrentQueue<RequestEntry> *target_array_ptr;
        RateLimiter _limiter;

        virtual void initial_ken_gen() = 0;

    public:
        Producer(int num, float qps, moodycamel::ConcurrentQueue<RequestEntry> &key_array)
                : _limiter(qps, default_time_window_size) {
            target_array_ptr = &key_array;
            _num = num;
            _gen = KeyGen();
        }


        Producer(int duration, int num, float qps, moodycamel::ConcurrentQueue<RequestEntry> &key_array)
                : _limiter(qps, default_time_window_size) {
            target_array_ptr = &key_array;
            _num = std::min(static_cast<int>(duration * qps), num);
            _gen = KeyGen();
        }

        Producer(const Producer &instance) : _limiter(instance._limiter.limit_,
                                                      instance._limiter.interval_) {
            _num = instance._num;
            _gen = instance._gen;
            worker_id = instance.worker_id;
            target_array_ptr = instance.target_array_ptr;
        }

        void setKeyGen(KeyGen keyGen) { _gen = KeyGen(keyGen); };

        static Producer *parsed_from_voidptr(void *arg);

        virtual pthread_t create_inserter() = 0;

        void fill_the_queue();
    };

    class Reader : Producer {
    private:
//        InserterArgs args;
        void initial_ken_gen() override;

    public:


        Reader(int duration,
               int num, float qps,
               moodycamel::ConcurrentQueue<RequestEntry> &key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }

        Reader(long num, float qps, moodycamel::ConcurrentQueue<RequestEntry> &queue_ptr)
                : Producer(num, qps, queue_ptr) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }

        pthread_t create_inserter() override;

        static void *reader_threads(void *args);
    };

    // size of one writer is 88, get by sizeof() function. small enough to maintain a vector.
    class Writer : Producer {
    private:
//        InserterArgs args;
        void initial_ken_gen() override;

    public:
        Writer(int duration,
               int num, float qps,
               moodycamel::ConcurrentQueue<RequestEntry> &key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kWrite);
            initial_ken_gen();
        }

        Writer(long num, float qps, moodycamel::ConcurrentQueue<RequestEntry> &queue_ptr)
                : Producer(num, qps, queue_ptr) {
            initial_ken_gen();
        }

        pthread_t create_inserter() override;

        static void *writer_threads(void *args);
    };

    class scenario {
        IX_NAME_SPACE::ms_clock::duration duration_seconds;

        scenario() : duration_seconds(10 * 1000) {};
    };

    class WorkloadEngine {
        std::vector<Reader> read_op_inserters;
        std::vector<Writer> write_op_inserters;

        std::vector<pthread_t> running_threads;

        moodycamel::ConcurrentQueue<RequestEntry> *buffer_queue;
    public:
        struct workload_tuple {
            int reader_num;
            int writer_num;

            workload_tuple(int a, int b) : reader_num(a), writer_num(b) {}
        };

        inline workload_tuple get_workload_size() {
            return WorkloadEngine::workload_tuple(read_op_inserters.size(),
                                                  write_op_inserters.size());
        }

        inline void create_entry_container() {
            this->buffer_queue = new moodycamel::ConcurrentQueue<RequestEntry>();
        }

        workload_tuple add_reader(Reader reader);

        workload_tuple add_reader(long num, float qps);

        workload_tuple add_writer(Writer writer);

        workload_tuple add_writer(long num, float qps);


        void start_workloads();

        void consume();


        WorkloadEngine() {
            read_op_inserters = std::vector<Reader>();
            write_op_inserters = std::vector<Writer>();
            create_entry_container();
        }

        ~WorkloadEngine() {
            read_op_inserters.clear();
            write_op_inserters.clear();
            delete this->buffer_queue;
        }

    };
};


#endif //IX_BENCHMARK_PRODUCER_H
