# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_task_three_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED task_three_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(task_three_FOUND FALSE)
  elseif(NOT task_three_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(task_three_FOUND FALSE)
  endif()
  return()
endif()
set(_task_three_CONFIG_INCLUDED TRUE)

# output package information
if(NOT task_three_FIND_QUIETLY)
  message(STATUS "Found task_three: 0.0.0 (${task_three_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'task_three' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${task_three_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(task_three_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${task_three_DIR}/${_extra}")
endforeach()
