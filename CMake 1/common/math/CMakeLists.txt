find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory(src math)

add_library(math STATIC ${math})

target_include_directories(math PUBLIC include)
target_link_libraries(math ${OpenCV_LIBS})