cmake_minimum_required(VERSION 3.16)
project(cache_optimization)

set(CMAKE_CXX_STANDARD 20)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_OPT_ARCH_NATIVE_SUPPORTED)
if (COMPILER_OPT_ARCH_NATIVE_SUPPORTED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
else()
    CHECK_CXX_COMPILER_FLAG("/arch:AVX2" COMPILER_OPT_ARCH_AVX_SUPPORTED)
    message("${COMPILER_OPT_ARCH_AVX_SUPPORTED}")
    if(COMPILER_OPT_ARCH_AVX_SUPPORTED)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
    endif()
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(SOURCES
        srcs/1_colwise_read.cpp
        srcs/2_rowwise_read.cpp
        srcs/3_unroll_loop.cpp
        srcs/4_vectorized_loop.cpp
        srcs/5_multithreaded.cpp
        srcs/6_vectorized_multithreaded.cpp)

set(HEADERS
        hdrs/types.h
        hdrs/timer.h
        hdrs/1_colwise_read.h
        hdrs/2_rowwise_read.h
        hdrs/3_unroll_loop.h
        hdrs/4_vectorized_loop.h
        hdrs/5_multithreaded.h
        hdrs/6_vectorized_multithreaded.h)

add_executable(cache_optimization main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(cache_optimization Threads::Threads)
