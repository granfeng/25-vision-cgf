cmake_minimum_required(VERSION 3.10)
project(Test)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
endif()

include_directories(${CMAKE_SOURCE_DIR}/common/rmath/include)
include_directories(${CMAKE_SOURCE_DIR}/common/singleton/include)
include_directories(${CMAKE_SOURCE_DIR}/modules/assembly1/include)
include_directories(${CMAKE_SOURCE_DIR}/modules/assembly2/include)
include_directories(${CMAKE_SOURCE_DIR}/modules/module1/include)
include_directories(${CMAKE_SOURCE_DIR}/modules/module2/include)

# # # # 以下内容禁止修改 # # # #
option(BUILD_A "build modules a" ON)
option(BUILD_B "build modules b" OFF)
option(BUILD_TESTS "build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
endif()

if(BUILD_A)
    add_definitions(-DWITH_A)
endif()
if(BUILD_B)
    add_definitions(-DWITH_B)
endif()

add_subdirectory(modules)
add_subdirectory(common)
# 小提示: module2 需要用到 pthread，即 target_link_libraries(xxx pthread)
# # # # 以上内容禁止修改 # # # #

add_executable(server server.cpp)
add_executable(client client.cpp)

target_include_directories(server PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(client PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(server PUBLIC rmath singleton assembly1 assembly2 module1 module2 ${OpenCV_LIBS})
target_link_libraries(client PUBLIC rmath singleton assembly1 assembly2 module1 module2 ${OpenCV_LIBS})

include(CTest)