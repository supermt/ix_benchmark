cmake_minimum_required(VERSION 3.16)
project(ix_benchmark)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES
        utils/rate_limiter.cpp
        include/rate_limiter.h
        container/op_bucket.cpp
        container/op_bucket.h
        include/format.h
        include/slice.h
        include/ix_namespace.h
        utils/slice.cpp
        generator/key_gen.h
        container/concurrentqueue.h)

find_package(Threads)

add_library(ix_interfaces INTERFACE)
target_link_libraries(ix_interfaces INTERFACE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(ix_interfaces INTERFACE include)

add_executable(producer_test producer_test.cpp ${SOURCES})
add_executable(ix_benchmark producer_test.cpp ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)

target_link_libraries(producer_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ix_benchmark ${CMAKE_THREAD_LIBS_INIT})
