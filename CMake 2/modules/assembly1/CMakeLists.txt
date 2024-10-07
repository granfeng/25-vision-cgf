aux_source_directory(src assembly1_Path1)
aux_source_directory(test assembly1_Path2)

add_library(assembly1 STATIC ${assembly1_Path1} ${assembly1_Path2})

target_include_directories(assembly1 PUBLIC ${CMAKE_SOURCE_DIR}/modules/assembly1/include)