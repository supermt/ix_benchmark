cmake_minimum_required(VERSION 3.17)
project(ix_benchmark)

set(CMAKE_CXX_STANDARD 14)

add_executable(ix_benchmark main.cpp rate_limiter.cpp rate_limiter.h)