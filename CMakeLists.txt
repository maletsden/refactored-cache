cmake_minimum_required(VERSION 3.16)
project(cache_optimization)

set(CMAKE_CXX_STANDARD 20)

add_executable(cache_optimization main.cpp srcs/1_colwise_read.cpp hdrs/types.h hdrs/1_colwise_read.h hdrs/timer.h srcs/2_rowwise_read.cpp hdrs/2_rowwise_read.h srcs/3_unroll_loop.cpp hdrs/3_unroll_loop.h hdrs/constants.h)