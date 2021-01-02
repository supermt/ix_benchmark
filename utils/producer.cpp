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
//                std::cout << _num << std::endl;
                target_array_ptr->enqueue(_gen.getNext());
                _num--;
            }
        }
    }


//    WorkloadEngine::workload_tuple WorkloadEngine::add_writer(Writer writer) {
//        assert(writer.worker_id == -1);
//        write_op_inserters.push_back(writer);
//        return get_workload_size();
//    }
//
//    WorkloadEngine::workload_tuple WorkloadEngine::add_reader(Reader reader) {
//        assert(reader.worker_id == -1);
//        read_op_inserters.push_back(reader);
//        return get_workload_size();
//    }

    void WorkloadEngine::start_workloads() {
        // 1. first step is to check through the pid of all workers. If there is
        // any thread's pid is -1, it means the target worker is not been started.
        if (read_op_inserters.size() + write_op_inserters.size() == running_threads.size()) {
            // all threads is running.
            return;
        }

        RequestQueue queue;
        int input_count = 10000;
        int i = 0;
        for (i = 0; i < 10; i++) {
            Reader reader(input_count, 10000, queue);
            reader.create_inserter();
        }

    }

    void WorkloadEngine::consume() {
        sleep(2);
//        RequestEntry temp;
//        while (_total_num > 0) {
//            if (_interrupt) {
//                break; // once the value is set to true, break the loop
//            }
//            while (buffer_queue->try_dequeue(temp)) {
//                // we won't interrupt the system until the last element being poped out.
//                // call the output function, this function can be set by another function pointer
//                std::cout << "key: " << temp._key << " type: " << temp._op << std::endl;
//                this->output_func(temp);
//                _total_num -= 1;
//                // thread-safely pop one out.
//            }
//
//        }

    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_reader(long num, float qps) {
        Reader temp = Reader(num, qps, *this->buffer_queue);
        return this->add_reader(temp);
    }

    WorkloadEngine::workload_tuple WorkloadEngine::add_writer(long num, float qps) {
        Writer temp = Writer(num, qps, *this->buffer_queue);
        return this->add_writer(temp);
    }

    void WorkloadEngine::default_output_function(RequestEntry single_request) {
        std::cout << single_request._key << " " << single_request._op << std::endl;
    }

    long WorkloadEngine::interrupt() {
//        std::cout << "task is interrupted" << std::endl;
        auto interrupted = this->_interrupt.load();
        // strongly set to broken
        this->_interrupt.compare_exchange_strong(interrupted, true);
        return _total_num;
    }


};

