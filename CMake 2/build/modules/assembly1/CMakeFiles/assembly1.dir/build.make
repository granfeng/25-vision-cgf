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
CMAKE_SOURCE_DIR = "/home/chen/CMake Task/CMake 2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/chen/CMake Task/CMake 2/build"

# Include any dependencies generated for this target.
include modules/assembly1/CMakeFiles/assembly1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include modules/assembly1/CMakeFiles/assembly1.dir/compiler_depend.make

# Include the progress variables for this target.
include modules/assembly1/CMakeFiles/assembly1.dir/progress.make

# Include the compile flags for this target's objects.
include modules/assembly1/CMakeFiles/assembly1.dir/flags.make

modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o: modules/assembly1/CMakeFiles/assembly1.dir/flags.make
modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o: ../modules/assembly1/src/assembly1.cpp
modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o: modules/assembly1/CMakeFiles/assembly1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/chen/CMake Task/CMake 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o"
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o -MF CMakeFiles/assembly1.dir/src/assembly1.cpp.o.d -o CMakeFiles/assembly1.dir/src/assembly1.cpp.o -c "/home/chen/CMake Task/CMake 2/modules/assembly1/src/assembly1.cpp"

modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/assembly1.dir/src/assembly1.cpp.i"
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/chen/CMake Task/CMake 2/modules/assembly1/src/assembly1.cpp" > CMakeFiles/assembly1.dir/src/assembly1.cpp.i

modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/assembly1.dir/src/assembly1.cpp.s"
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/chen/CMake Task/CMake 2/modules/assembly1/src/assembly1.cpp" -o CMakeFiles/assembly1.dir/src/assembly1.cpp.s

# Object files for target assembly1
assembly1_OBJECTS = \
"CMakeFiles/assembly1.dir/src/assembly1.cpp.o"

# External object files for target assembly1
assembly1_EXTERNAL_OBJECTS =

modules/assembly1/libassembly1.a: modules/assembly1/CMakeFiles/assembly1.dir/src/assembly1.cpp.o
modules/assembly1/libassembly1.a: modules/assembly1/CMakeFiles/assembly1.dir/build.make
modules/assembly1/libassembly1.a: modules/assembly1/CMakeFiles/assembly1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/chen/CMake Task/CMake 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libassembly1.a"
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && $(CMAKE_COMMAND) -P CMakeFiles/assembly1.dir/cmake_clean_target.cmake
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/assembly1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/assembly1/CMakeFiles/assembly1.dir/build: modules/assembly1/libassembly1.a
.PHONY : modules/assembly1/CMakeFiles/assembly1.dir/build

modules/assembly1/CMakeFiles/assembly1.dir/clean:
	cd "/home/chen/CMake Task/CMake 2/build/modules/assembly1" && $(CMAKE_COMMAND) -P CMakeFiles/assembly1.dir/cmake_clean.cmake
.PHONY : modules/assembly1/CMakeFiles/assembly1.dir/clean

modules/assembly1/CMakeFiles/assembly1.dir/depend:
	cd "/home/chen/CMake Task/CMake 2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/chen/CMake Task/CMake 2" "/home/chen/CMake Task/CMake 2/modules/assembly1" "/home/chen/CMake Task/CMake 2/build" "/home/chen/CMake Task/CMake 2/build/modules/assembly1" "/home/chen/CMake Task/CMake 2/build/modules/assembly1/CMakeFiles/assembly1.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : modules/assembly1/CMakeFiles/assembly1.dir/depend

