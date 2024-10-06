# 在此处进行 CMake 设置（最小版本号，项目名）
cmake_minimum_required(VERSION 3.10)
project(Test)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
endif()

####### 以下禁止修改 #######
add_subdirectory(common)   #
add_subdirectory(modules)  #
####### 以上禁止修改 #######

# 在此处生成可执行程序
add_executable(main main.cpp)

target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})

# 在此处链接库

target_link_libraries(main PRIVATE A1 A2 M1 M2 math ${OpenCV_LIBS})
