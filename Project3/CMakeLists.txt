cmake_minimum_required(VERSION 3.8)
project(Project3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp fib_heap.h binary_heap.h priority_queue.h unsorted_heap.h)
add_executable(Project3 ${SOURCE_FILES})