//
// Created by jinghuan on 12/30/20.
//

#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {
    pthread_t writer::create_inserter() {
        pthread_create(&worker_id, NULL, this->writer_threads, this);
        std::cout << "start the writer at pid: " << this->worker_id << std::endl;
        return this->worker_id;
    }

    void *writer::writer_threads(void *args) {
        producer *worker = producer::parsed_from_voidptr(args);
        if (worker->depathed_or_not) {
            pthread_detach(worker->worker_id);
        }
        worker->_gen.set_op(kWrite);
        worker->fill_the_queue();
        return NULL;
    }
};
