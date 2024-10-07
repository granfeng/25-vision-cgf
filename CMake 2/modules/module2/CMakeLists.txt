cmake_minimum_required(VERSION 3.10)

project(module2)

aux_source_directory(src module2_Path)

add_library(module2 STATIC ${module2_Path})

target_include_directories(module2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)