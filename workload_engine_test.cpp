//
// Created by jinghuan on 1/2/21.
//

#include "include/producer.h"
#include <iostream>
#include <unistd.h>


int main() {
    using namespace IX_NAME_SPACE;

    WorkloadEngine test_engine;
    test_engine.add_reader(100, 10);
//    test_engine.add_writer(2000, 200);

//    pthread_exit(NULL);
    test_engine.start_workloads();
    test_engine.consume();
    sleep(1);
    test_engine.interrupt();

//    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;
}
