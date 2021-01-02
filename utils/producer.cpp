//
// Created by jinghuan on 12/30/20.
//
#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {

    Producer *Producer::parsed_from_voidptr(void *arg) {
        return (Producer *) arg;
    }

    void Producer::fill_the_queue() {
        while (_num >= 0) {
            if (_limiter.reqeust()) {
                target_array_ptr->enqueue(_gen.getNext());
                _num--;
            }
        }
    }


    WorkloadEngine::workload_tuple WorkloadEngine::add_writer(Writer writer) {
        write_op_inserters.push_back(writer);
        return get_workload_size();
    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_reader(Reader reader) {
        return get_workload_size();
    }

    void WorkloadEngine::start_workloads() {

    }

    void WorkloadEngine::consume() {

    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_reader(long num, float qps) {
        Reader temp = Reader(num, qps, *this->buffer_queue);
        this->read_op_inserters.push_back(temp);
        return get_workload_size();
    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_writer(long num, float qps) {
        Writer temp = Writer(num,qps,*this->buffer_queue);
        return get_workload_size();
    }
};

