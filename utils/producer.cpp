//
// Created by jinghuan on 12/30/20.
//
#include "include/producer.h"
#include <iostream>
#include <unistd.h>

namespace IX_NAME_SPACE {

    Producer *Producer::parsed_from_voidptr(void *arg) {
        return (Producer *) arg;
    }

    void Producer::fill_the_queue() {
        int num = _num;
        while (num >= 0) {
            _limiter->aquire();
            target_array_ptr->enqueue(_gen.getNext());
            num--;
        }
    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_reader(long num, float qps) {
        Reader temp = Reader(num, qps, NULL);
        return this->add_reader(temp);
    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_writer(long num, float qps) {
        Writer temp = Writer(num, qps, NULL);
        return this->add_writer(temp);
    }

    void WorkloadEngine::default_output_function(RequestEntry single_request) {
        std::cout << "op key is " << single_request._key.to_string() << std::endl;
    }

    struct QueueWithNum {
        RequestQueue *arg;
        long total_num;
        WorkloadEngine *engine;
    };

    void *WorkloadEngine::default_consumer(void *arg) {
        QueueWithNum *parsed_args = (QueueWithNum *) arg;
        RequestQueue *buffer_queue = parsed_args->arg;
        RequestEntry item;

        int num = parsed_args->total_num;// stop after we get 10 items;
        while (true) {
            if (buffer_queue->size_approx() > 0) {
                if (buffer_queue->try_dequeue(item)) {
                    parsed_args->engine->output_func(item);
                    num--;
                }
            }
            if (num < 0) {
                break;
            }
        }
        return NULL;
    }

    void WorkloadEngine::consume() {
        RequestQueue *queue = new RequestQueue();
        int input_count = 0;
        std::vector<std::thread> thread_pool;
        for (Reader tmp: this->read_op_inserters) {
            input_count += tmp._num;
            tmp.target_array_ptr = queue;
            thread_pool.push_back(tmp.create_inserter());
        }
        for (Writer tmp: this->write_op_inserters) {
            input_count += tmp._num;
            tmp.target_array_ptr = queue;
            thread_pool.push_back(tmp.create_inserter());
        }
        struct QueueWithNum arg{queue, input_count, this};
        std::thread consumer = std::thread(default_consumer, &arg);

        consumer.join();
        for (int i = 0; i < thread_pool.size(); i++) {
            thread_pool[i].join();
        }

    }

    void BootstrapTheEngine(WorkloadEngine &test_engine) {
//        RequestQueue *queue = new RequestQueue();
//        int input_count = 0;
//        std::vector<std::thread> thread_pool;
//        for (Reader tmp:*test_engine.getReaders()) {
//            input_count += tmp._num;
//            tmp.target_array_ptr = queue;
//            thread_pool.push_back(tmp.create_inserter());
//
//        }
////        for (Writer wr_tmp:*test_engine.getWriters()) {
////            input_count += wr_tmp._num;
////            wr_tmp.target_array_ptr = queue;
////            wr_tmp.create_inserter();
////        }
//
//        std::thread consumer;
//        consumer = std::thread(WorkloadEngine::default_consumer, queue);
//        thread_pool.push_back(consumer);
//        std::cout << input_count << " entries need to pop" << std::endl;
//        RequestEntry current_request;
////
////        consumer.detach();
//        for (int thread_count = 0; thread_count < thread_pool.size(); thread_count++) {
//            thread_pool[thread_count].join();
//        }
//        consumer.join();
//
//        RequestEntry item;

    }

}; // end namespace

