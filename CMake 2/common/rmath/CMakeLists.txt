find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory(src rmath_Path)

add_library(rmath STATIC ${rmath_Path})

target_include_directories(rmath PUBLIC ${CMAKE_SOURCE_DIR}/common/rmath/include)
target_link_libraries(math ${OpenCV_LIBS})
