//
// Created by jinghuan on 12/30/20.
//

#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {
    pthread_t Writer::create_inserter() {
        pthread_create(&worker_id, NULL, this->writer_threads, this);
        std::cout << "start the Writer at pid: " << this->worker_id << std::endl;
        return this->worker_id;
    }

    void *Writer::writer_threads(void *args) {
        Producer *worker = Producer::parsed_from_voidptr(args);
        if (worker->depathed_or_not) {
            pthread_detach(worker->worker_id);
        }
        worker->_gen.set_op(kWrite);
        worker->fill_the_queue();
        return NULL;
    }

    void Writer::initial_ken_gen() {
        _gen = KeyGen(kWrite);

    }
};
