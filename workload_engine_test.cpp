//
// Created by jinghuan on 1/2/21.
//

#include "include/producer.h"
#include <iostream>
#include <unistd.h>


int main() {
    using namespace IX_NAME_SPACE;

//    RequestQueue queue;
//    int input_count = 10000;
//    int i = 0;
//    for (i = 0; i < 10; i++) {
//        Reader reader(input_count, 10000, queue);
//        reader.create_inserter();
//    }
//
//    int num = i * input_count;
//    RequestEntry tmp;
//    while (num > 0) {
//        while (queue.try_dequeue(tmp)) {
//            std::cout << num << std::endl;
//            num--;
//        }
//    }

//    sleep(10);
//    std::cout << queue.size_approx() << std::endl;
    WorkloadEngine test_engine;
//    test_engine.add_reader(100, 1000);
//    test_engine.add_writer(2000, 200);
//
////    pthread_exit(NULL);
    test_engine.start_workloads();
    test_engine.consume();
//    sleep(1);
//    test_engine.interrupt();

//    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;
}
