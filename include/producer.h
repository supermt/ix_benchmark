//
// Created by jinghuan on 12/30/20.
//

#ifndef IX_BENCHMARK_PRODUCER_H
#define IX_BENCHMARK_PRODUCER_H
#pragma once

#include "rate_limiter.h"
#include <pthread.h>
#include "generator/key_gen.h"
#include "container/concurrentqueue.h" // this lib is not that reliable

#include <vector>
#include <map>
#include <algorithm>

namespace IX_NAME_SPACE {
    // default time window with duration of one second
    static auto default_time_window_size = ms_clock::duration(1000);

    class Reader;

    class Writer;

    class Producer;

    class WorkloadEngine;

    class scenario;

    typedef moodycamel::ConcurrentQueue<RequestEntry> RequestQueue;

    class Producer {
    public:
        std::atomic<long> _num;
        KeyGen _gen;
        const bool depathed_or_not = true; // just for cases

        RequestQueue *target_array_ptr;
        RateLimiter _limiter;

        virtual void initial_ken_gen() = 0;

        pthread_t worker_id = -1;
    public:
        Producer(int num, float qps, RequestQueue &key_array)
                : _limiter(qps, default_time_window_size) {
            target_array_ptr = &key_array;
            _num = num;
            _gen = KeyGen();
        }


        Producer(int duration, int num, float qps, RequestQueue &key_array)
                : _limiter(qps, default_time_window_size) {
            target_array_ptr = &key_array;
            _num = std::min(static_cast<int>(duration * qps), num);
            _gen = KeyGen();
        }

        Producer(const Producer &instance) : _limiter(instance._limiter.limit_,
                                                      instance._limiter.interval_) {
            _num = instance._num.load();
            _gen = instance._gen;
            worker_id = instance.worker_id;
            target_array_ptr = instance.target_array_ptr;
        }

        void setKeyGen(KeyGen keyGen) { _gen = KeyGen(keyGen); };

        static Producer *parsed_from_voidptr(void *arg);

        virtual pthread_t create_inserter() = 0;

        void fill_the_queue();
    };

    class Reader : public Producer {
    private:
//        InserterArgs args;
        void initial_ken_gen() override;

    public:
        Reader(int duration,
               int num, float qps,
               RequestQueue &key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }

        Reader(long num, float qps, RequestQueue &queue_ptr)
                : Producer(num, qps, queue_ptr) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }


        pthread_t create_inserter() override;

        static void *reader_threads(void *args);
    };

    // size of one writer is 88, get by sizeof() function. small enough to maintain a vector.
    class Writer : public Producer {
    private:
//        InserterArgs args;
        void initial_ken_gen() override;

    public:
        Writer(int duration,
               int num, float qps,
               RequestQueue &key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kWrite);
            initial_ken_gen();
        }

        Writer(long num, float qps, RequestQueue &queue_ptr)
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

        std::map<pthread_t, Producer *> running_threads;

        RequestQueue *buffer_queue;

        std::atomic<bool> _interrupt;
        std::atomic<long> _total_num;

    public:
        void (*output_func)(RequestEntry single_request);

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
            this->buffer_queue = new RequestQueue();
        }

        inline workload_tuple add_reader(Reader &reader) {
            read_op_inserters.push_back(reader);
//            _total_num += reader._num;
            return get_workload_size();
        }

        inline workload_tuple add_writer(Writer &writer) {
            write_op_inserters.push_back(writer);
//            _total_num += writer._num;
            return get_workload_size();
        }

        workload_tuple add_reader(long num, float qps);

        workload_tuple add_writer(long num, float qps);

        inline std::vector<Reader> *getReaders() { return &this->read_op_inserters; }

        inline std::vector<Writer> *getWriters() { return &this->write_op_inserters; }

        WorkloadEngine() : _interrupt(false), _total_num(0) {
            read_op_inserters = std::vector<Reader>();
            write_op_inserters = std::vector<Writer>();
            output_func = default_output_function; // the function must be static!
        }

        ~WorkloadEngine() {
            read_op_inserters.clear();
            write_op_inserters.clear();
        }

        //        static void *output_function(void *args);
        static void default_output_function(RequestEntry single_request);

        void set_output_func(void (*target_function)(RequestEntry)) {
            this->output_func = target_function;
        }

    };


    // end of Workload Engine
    static void BootstrapTheEngine(WorkloadEngine &test_engine);
}; // end namespace


#endif //IX_BENCHMARK_PRODUCER_H
