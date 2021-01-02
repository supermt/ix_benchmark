//
// Created by jinghuan on 1/2/21.
//

#include "include/producer.h"
#include <iostream>


int main() {
    using namespace IX_NAME_SPACE;

    WorkloadEngine test_engine;
    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;
    
    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;

    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;
}
