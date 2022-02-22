cmake_minimum_required(VERSION 3.20)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)
include_directories(${data})
add_executable(
    ${CMAKE_PROJECT_NAME}
    stb_image_resize.h 
    stb_image_write.h 
    stb_image.h 
    utils.h
    histogram.h 
    images.h 
    histogram.cpp
    images.cpp 
    main.cpp
    )
    
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.exe DESTINATION ${CMAKE_SOURCE_DIR})