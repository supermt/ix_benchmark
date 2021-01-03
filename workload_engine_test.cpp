//
// Created by jinghuan on 1/2/21.
//

#include "include/producer.h"
#include <iostream>
#include <unistd.h>


int main() {
    using namespace IX_NAME_SPACE;

//    sleep(10);
//    std::cout << queue.size_approx() << std::endl;
    WorkloadEngine test_engine;
    test_engine.add_reader(1000, 200);
    test_engine.add_reader(1000, 200);
    test_engine.add_writer(1000, 200);
    //    RequestQueue queue2;
//    Reader reader = Reader(1000, 100, &queue2);
//    reader.create_inserter();
////    test_engine.add_writer(200, 100);
//    int num = 1000;
//    RequestEntry temp;
//    while (num > 0) {
//        while (queue2.try_dequeue(temp)) {
//            std::cout << "dequeue the entry: " << std::fixed << temp._key.to_string() << " entry seq: " << num
//                      << std::endl;
//            num--;
//        };
//    }
//    BootstrapTheEngine(test_engine);
    test_engine.consume();
}

