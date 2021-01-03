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
        while (_num >= 0) {
            if (_limiter.reqeust()) {
                std::cout << _num << std::endl;
                target_array_ptr->enqueue(_gen.getNext());
                _num--;
            }
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
        std::cout << single_request._op << std::endl;
    }


    void BootstrapTheEngine(WorkloadEngine &test_engine) {
        RequestQueue queue;
        int input_count = 0;
        for (Reader tmp:*test_engine.getReaders()) {
            input_count += tmp._num;
            tmp.target_array_ptr = &queue;
            tmp.create_inserter();
        }


        for (Writer wr_tmp:*test_engine.getWriters()) {
            input_count += wr_tmp._num;
            wr_tmp.target_array_ptr = &queue;
            wr_tmp.create_inserter();
        }
        std::cout << input_count << " entries need to pop" << std::endl;
        RequestEntry current_request;
        sleep(10);

//        while (input_count > 0) {
//            while (queue.try_dequeue(current_request)) {
//                test_engine.output_func(current_request);
//                input_count--;
//            }
//        }

    }

}; // end namespace

