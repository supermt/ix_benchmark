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
    test_engine.add_reader(100, 20);
    test_engine.add_reader(100, 20);
    test_engine.add_writer(200, 10);

    BootstrapTheEngine(test_engine);
}
