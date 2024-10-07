include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(singleton INTERFACE)

target_include_directories(singleton INTERFACE ${CMAKE_SOURCE_DIR}/include)