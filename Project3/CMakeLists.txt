cmake_minimum_required(VERSION 3.8)
project(Project3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES binary_heap.h unsorted_heap.h fib_heap.h test_heap.cpp main.cpp)
add_executable(Project3 ${SOURCE_FILES})