//
// Created by jinghuan on 12/30/20.
//

#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {

    void *Writer::writer_threads(void *args) {
        Producer *worker = Producer::parsed_from_voidptr(args);
        worker->_gen.set_op(kWrite);
        worker->fill_the_queue();
        return NULL;
    }

    std::thread Writer::create_inserter() {
        std::thread working_thread;
        working_thread = std::thread(writer_threads, this);
//        if (depathed_or_not) {
//            working_thread.detach();
//        } else {
//            working_thread.join();
//        }
        std::cout << "start the Writer at pid: " << this->worker_id << std::endl;
        return working_thread;
    }

    void Writer::initial_ken_gen() {
        _gen = KeyGen(kWrite);

    }
};
