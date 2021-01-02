cmake_minimum_required(VERSION 3.17)
project(ix_benchmark)

set(CMAKE_CXX_STANDARD 14)

include_directories(
    "${PROJECT_BINARY_DIR}/include"
    "."
)

add_library(ix_benchmark "" utils/reader.cpp utils/writer.cpp utils/producer.cpp)
target_sources(ix_benchmark
        PRIVATE
        "utils/rate_limiter.cpp"
        "utils/slice.cpp"
        "generator/key_gen.h"
        "utils/op_bucket.cpp"
        )

target_include_directories(ix_benchmark
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        )

find_package(Threads REQUIRED)
target_link_libraries(ix_benchmark Threads::Threads)

add_executable(producer_test "producer_test.cpp")
add_executable(workload_engine_test "workload_engine_test.cpp")

target_link_libraries(producer_test ix_benchmark)
