find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${CMAKE_SOURCE_DIR}/modules/assembly2/include)

aux_source_directory(src/module1a module1_Path1)
aux_source_directory(src/module1b module1_Path2)

add_library(module1 STATIC ${module1_Path} ${module1_Path2})

target_include_directories(module1 PUBLIC include)
target_link_libraries(module1 ${OpenCV_LIBS})