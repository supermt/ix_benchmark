cmake_minimum_required(VERSION 3.17)
project(ix_benchmark)

set(CMAKE_CXX_STANDARD 14)

add_executable(ix_benchmark main.cpp rate_limiter.cpp include/rate_limiter.h op_bucket.cpp op_bucket.h include/format.h include/slice.h include/ix_namespace.h slice.cpp)