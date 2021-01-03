//
// Created by jinghuan on 12/30/20.
//

#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {

    void *Reader::reader_threads(void *arg) {
        Producer *worker = Producer::parsed_from_voidptr(arg);
        worker->_gen.set_op(kQuery);
        worker->fill_the_queue();
        return NULL;
    }

    pthread_t Reader::create_inserter() {
        std::thread working_thread;
        working_thread = std::thread(reader_threads, this);
//        working_thread.join();
        if (depathed_or_not) {
            working_thread.detach();
        } else {
            working_thread.join();
        }
//        pthread_create(&worker_id, NULL, this->reader_threads, this);
        std::cout << "start the Reader at pid: " << this->worker_id << std::endl;
        return this->worker_id;
    }

    void Reader::initial_ken_gen() {
        _gen = KeyGen(kQuery);
    }

};