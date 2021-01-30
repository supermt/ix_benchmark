//
// Created by jinghuan on 12/30/20.
//

#include "../include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {

    void *Reader::reader_threads(void *arg) {
        Producer *worker = Producer::parsed_from_voidptr(arg);
        if (worker->depathed_or_not) {
            pthread_detach(worker->worker_id);
        }
        worker->_gen.set_op(kQuery);
        worker->fill_the_queue();
        return NULL;
    }

    pthread_t Reader::create_inserter() {
        pthread_create(&worker_id, NULL, this->reader_threads, this);
        std::cout << "start the Reader at pid: " << this->worker_id << std::endl;
        return this->worker_id;
    }

    void Reader::initial_ken_gen() {
        _gen = KeyGen(kQuery);
    }

};