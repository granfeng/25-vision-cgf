cmake_minimum_required(VERSION 3.10)
project(Test)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
endif()

# # # # 以下内容禁止修改 # # # #
option(BUILD_A "build modules a" ON)
option(BUILD_B "build modules b" OFF)
option(BUILD_TESTS "build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(modules)
add_subdirectory(common)
# 小提示: module2 需要用到 pthread，即 target_link_libraries(xxx pthread)
# # # # 以上内容禁止修改 # # # #

add_executable(server server.cpp)
add_executable(client client.cpp)

target_include_directories(server PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(client PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(main PUBLIC A1 A2 M1 M2 math ${OpenCV_LIBS})
