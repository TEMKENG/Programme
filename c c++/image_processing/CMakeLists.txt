cmake_minimum_required(VERSION 3.20)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)
include_directories(${data})
add_executable(image_processing stb_image_resize.h stb_image_write.h stb_image.h utils.h Histogram.h  Image.h  Histogram.cpp Image.cpp utils.cpp images.cpp images.h main.cpp)
