# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chen/Task/navigation_ws/src/task_one

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chen/Task/navigation_ws/build/task_one

# Utility rule file for ament_cmake_python_copy_task_one_setup.

# Include any custom commands dependencies for this target.
include CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/progress.make

CMakeFiles/ament_cmake_python_copy_task_one_setup:
	/usr/bin/cmake -E copy /home/chen/Task/navigation_ws/src/task_one/setup.cfg /home/chen/Task/navigation_ws/build/task_one/ament_cmake_python/task_one/setup.cfg

ament_cmake_python_copy_task_one_setup: CMakeFiles/ament_cmake_python_copy_task_one_setup
ament_cmake_python_copy_task_one_setup: CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/build.make
.PHONY : ament_cmake_python_copy_task_one_setup

# Rule to build all files generated by this target.
CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/build: ament_cmake_python_copy_task_one_setup
.PHONY : CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/build

CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/clean

CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/depend:
	cd /home/chen/Task/navigation_ws/build/task_one && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chen/Task/navigation_ws/src/task_one /home/chen/Task/navigation_ws/src/task_one /home/chen/Task/navigation_ws/build/task_one /home/chen/Task/navigation_ws/build/task_one /home/chen/Task/navigation_ws/build/task_one/CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ament_cmake_python_copy_task_one_setup.dir/depend

