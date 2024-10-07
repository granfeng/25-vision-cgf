find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory(src assembly2_Path)

add_library(assembly2 STATIC ${assembly2_Path})

target_include_directories(assembly2 PUBLIC include)
target_link_libraries(math ${OpenCV_LIBS})