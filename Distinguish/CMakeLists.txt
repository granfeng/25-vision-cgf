cmake_minimum_required(VERSION 3.10)
project(Distinguish)

set(CMAKE_CXX_STANDARD 11)

# 添加 OpenCV 库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Distinguish Distinguish.cpp)

# 链接 OpenCV、Tesseract 库
target_link_libraries(Distinguish ${OpenCV_LIBS})