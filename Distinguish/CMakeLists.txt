cmake_minimum_required(VERSION 3.10)
project(Distinguish)

set(CMAKE_CXX_STANDARD 11)

# 添加 OpenCV 库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# 添加 Tesseract 库
#设置路径
set(Tesseract_INCLUDE_DIR /usr/include)
set(Tesseract_LIBRARY /usr/lib/x86_64-linux-gnu/libtesseract.so)
include_directories(${Tesseract_INCLUDE_DIR})
link_directories(${Tesseract_LIBRARY})

add_executable(Distinguish Distinguish.cpp)

# 链接 OpenCV、Tesseract 库
target_link_libraries(Distinguish ${OpenCV_LIBS})
target_link_libraries(Distinguish ${Tesseract_LIBRARY})