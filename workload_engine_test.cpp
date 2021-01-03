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
    test_engine.add_reader(100, 1000);
    test_engine.add_reader(100, 1000);
    test_engine.add_writer(2000, 200);
//
////    pthread_exit(NULL);
//    sleep(1);
//    test_engine.interrupt();

    std::cout << sizeof(test_engine) << " " << test_engine.get_workload_size().reader_num << std::endl;

    RequestQueue queue;
    int input_count = 10000;
    int i = 0;
    for (i = 0; i < test_engine.get_workload_size().reader_num; i++) {
        Reader reader(input_count, 10000, queue);
        reader.create_inserter();
    }

    for (i = 0; i < test_engine.get_workload_size().writer_num; i++) {
        Writer writer(input_count, 10000, queue);
        writer.create_inserter();
    }

    std::cout << "created " << std::endl;
    int num = (test_engine.get_workload_size().reader_num + test_engine.get_workload_size().writer_num) * input_count;
    RequestEntry tmp;
    int j;
    for (j = 0; j < num; j++) {
        while (queue.try_dequeue(tmp)) {
            std::cout << "entry key " << tmp._key << std::endl;
        }
    }
    std::cout << "total entry count " << j << std::endl;

}
