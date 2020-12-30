//
// Created by jinghuan on 12/30/20.
//
#include "include/producer.h"
#include <iostream>

namespace IX_NAME_SPACE {

//    InserterArgs producer::parsed_from_voidptr(void *arg) {
//        InserterArgs result = *((InserterArgs *) arg);
//        return result;
//    }


    producer *producer::parsed_from_voidptr(void *arg) {
        return (producer *) arg;
    }

    void producer::fill_the_queue() {
        while (_num >= 0) {
            if (_limiter.reqeust()) {
                target_array_ptr->enqueue(_gen.getNext());
                _num--;
            }
        }
    }

};

