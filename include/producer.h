//
// Created by jinghuan on 12/30/20.
//

#ifndef IX_BENCHMARK_PRODUCER_H
#define IX_BENCHMARK_PRODUCER_H
#pragma once

#include "rate_limiter/rate_limiter_interface.h"
#include "rate_limiter/rate_limiter.h"

#include <pthread.h>
#include "generator/key_gen.h"
#include "container/concurrentqueue.h" // this lib is not that reliable

#include <vector>
#include <map>
#include <algorithm>

namespace IX_NAME_SPACE {
    // default time window with duration of one second
    static auto default_time_window_size = 1;//ms_clock::duration(1000);


    class Reader;

    class Writer;

    class Producer;

    class WorkloadEngine;

    class scenario;

    typedef moodycamel::ConcurrentQueue<RequestEntry> RequestQueue;

    class Producer {
    public:
        long _num;
        KeyGen _gen;
        const bool depathed_or_not = true; // just for cases

        RequestQueue *target_array_ptr;
        IX_NAME_SPACE::RateLimiterInterface *_limiter;

        virtual void initial_ken_gen() = 0;

        pthread_t worker_id = -1;
    public:
        Producer(int num, float qps, RequestQueue *key_array) {
            target_array_ptr = key_array;
            _num = num;
            _gen = KeyGen();
            _limiter = new RateLimiter();
            _limiter->set_rate(qps);
        }

        Producer(const Producer &instance) {
            _limiter = new RateLimiter();
            _limiter->set_rate(instance._limiter->get_rate());
            _num = instance._num;
            _gen = instance._gen;
        };


        Producer(int duration, int num, float qps, RequestQueue *key_array) {
            _limiter = new RateLimiter();
            _limiter->set_rate(qps);
            target_array_ptr = key_array;
            _num = std::max(static_cast<int>(duration * qps), num);
            _gen = KeyGen();
        }


        void setKeyGen(KeyGen keyGen) { _gen = KeyGen(keyGen); };

        static Producer *parsed_from_voidptr(void *arg);

        virtual std::thread create_inserter() = 0;

        void fill_the_queue();
    };

    class Reader : public Producer {
    private:
//        InserterArgs args;
        void initial_ken_gen() override;

    public:
        Reader(int duration,
               int num, float qps,
               RequestQueue *key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }

        Reader(long num, float qps, RequestQueue *queue_ptr)
                : Producer(num, qps, queue_ptr) {
//            _gen = KeyGen(kQuery);
            initial_ken_gen();
        }


        std::thread create_inserter() override;

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
               RequestQueue *key_array) :
                Producer(duration, num, qps, key_array) {
//            _gen = KeyGen(kWrite);
            initial_ken_gen();
        }

        Writer(long num, float qps, RequestQueue *queue_ptr)
                : Producer(num, qps, queue_ptr) {
            initial_ken_gen();
        }

        std::thread create_inserter() override;

        static void *writer_threads(void *args);
    };


    class WorkloadEngine {
        std::vector<Reader> read_op_inserters;
        std::vector<Writer> write_op_inserters;

        std::map<pthread_t, Producer *> running_threads;

        std::atomic<bool> _interrupt;
        long _total_num;

    public:
        void (*output_func)(RequestEntry single_request);

    public:
        struct workload_tuple {
            int reader_num;
            int writer_num;

            workload_tuple(int a, int b) : reader_num(a), writer_num(b) {}
        };

        inline std::map<pthread_t, Producer *> get_thread_map() {
            return running_threads;
        }

        inline workload_tuple get_workload_size() {
            return WorkloadEngine::workload_tuple(read_op_inserters.size(),
                                                  write_op_inserters.size());
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
        static void *default_consumer(void* arg);
        void set_output_func(void (*target_function)(RequestEntry)) {
            this->output_func = target_function;
        }

        void consume();

    };

    void BootstrapTheEngine(WorkloadEngine &test_engine);


}; // end namespace


#endif //IX_BENCHMARK_PRODUCER_H
