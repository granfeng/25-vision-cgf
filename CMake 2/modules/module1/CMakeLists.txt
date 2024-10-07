aux_source_directory(src module1_Path)

add_library(module1 STATIC ${module1_Path})

target_include_directories(module1 PUBLIC include)