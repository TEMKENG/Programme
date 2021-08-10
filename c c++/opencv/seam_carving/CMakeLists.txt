cmake_minimum_required(VERSION 3.19)
project(seam_carving)
include(CTest)
enable_testing()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(data)
add_executable(seam_carving main.cpp)

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Additional Library Directories
link_directories( ${OpenCV_LIB_DIR} )

# Additional Dependencies
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)