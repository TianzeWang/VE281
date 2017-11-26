cmake_minimum_required(VERSION 3.8)
project(Project3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES test_heap.cpp fib_heap2.h)
add_executable(Project3 ${SOURCE_FILES})