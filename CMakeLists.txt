cmake_minimum_required(VERSION 3.17)
project(ix_benchmark)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_XCODE_GENERATE_SCHEME ON)
set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER ON)
set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER_USE_AFTER_RETURN ON)

include_directories(
    "${PROJECT_BINARY_DIR}/include"
    "."
)

add_library(ix_benchmark "" )
target_sources(ix_benchmark
        PRIVATE
        "utils/rate_limiter.cpp"
        "utils/rate_limiter.cpp"
        "utils/slice.cpp"
        "generator/key_gen.h"
        "utils/op_bucket.cpp"
        "utils/reader.cpp"
        "utils/writer.cpp"
        "utils/producer.cpp"
        "utils/key.cpp"
        )

target_include_directories(ix_benchmark
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        )
# add this line, otherwise, it will lead to "undefined reference" false.
link_libraries(ix_benchmark)

# add needed dependencies
find_package(Threads REQUIRED)
target_link_libraries(ix_benchmark Threads::Threads)


add_executable(producer_test "producer_test.cpp")
add_executable(workload_engine_test "workload_engine_test.cpp")

target_link_libraries(producer_test ix_benchmark)
