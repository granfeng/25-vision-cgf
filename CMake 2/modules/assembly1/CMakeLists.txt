aux_source_directory(src assembly1_Path1)

add_library(assembly1 STATIC ${assembly1_Path1})

target_include_directories(assembly1 PUBLIC ${CMAKE_SOURCE_DIR}/modules/assembly1/include)

