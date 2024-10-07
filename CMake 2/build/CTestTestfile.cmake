# CMake generated Testfile for 
# Source directory: /home/chen/CMake Task/CMake 2
# Build directory: /home/chen/CMake Task/CMake 2/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ctest "/home/chen/CMake Task/CMake 2/build/my_test")
set_tests_properties(ctest PROPERTIES  _BACKTRACE_TRIPLES "/home/chen/CMake Task/CMake 2/CMakeLists.txt;40;add_test;/home/chen/CMake Task/CMake 2/CMakeLists.txt;0;")
subdirs("modules")
subdirs("common")
