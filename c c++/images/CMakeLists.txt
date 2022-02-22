cmake_minimum_required(VERSION 3.20)
project(images)

set(CMAKE_CXX_STANDARD 20)

add_executable(images main.cpp utils.cpp utils.h histogram.cpp histogram.h)
