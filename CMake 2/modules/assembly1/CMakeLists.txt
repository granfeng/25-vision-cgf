cmake_minimum_required(VERSION 3.10)

project(assembly1)

aux_source_directory(src assembly1_Path1)

add_library(assembly1 STATIC ${assembly1_Path1})

target_include_directories(assembly1 PUBLIC ${CMAKE_SOURCE_DIR}/modules/assembly1/include)

add_executable(assembly1_test test/assembly1_test.cpp)
target_include_directories(assembly1_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules/assembly1/include)
target_link_libraries(assembly1_test PUBLIC assembly1)
add_test(NAME assembly1_test COMMAND assembly1_test)