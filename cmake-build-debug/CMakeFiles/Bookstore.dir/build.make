# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/14908/Desktop/Bookstore

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Bookstore.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Bookstore.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Bookstore.dir/flags.make

CMakeFiles/Bookstore.dir/code/main.cpp.o: CMakeFiles/Bookstore.dir/flags.make
CMakeFiles/Bookstore.dir/code/main.cpp.o: ../code/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Bookstore.dir/code/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Bookstore.dir/code/main.cpp.o -c /mnt/c/Users/14908/Desktop/Bookstore/code/main.cpp

CMakeFiles/Bookstore.dir/code/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bookstore.dir/code/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/14908/Desktop/Bookstore/code/main.cpp > CMakeFiles/Bookstore.dir/code/main.cpp.i

CMakeFiles/Bookstore.dir/code/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bookstore.dir/code/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/14908/Desktop/Bookstore/code/main.cpp -o CMakeFiles/Bookstore.dir/code/main.cpp.s

# Object files for target Bookstore
Bookstore_OBJECTS = \
"CMakeFiles/Bookstore.dir/code/main.cpp.o"

# External object files for target Bookstore
Bookstore_EXTERNAL_OBJECTS =

Bookstore: CMakeFiles/Bookstore.dir/code/main.cpp.o
Bookstore: CMakeFiles/Bookstore.dir/build.make
Bookstore: CMakeFiles/Bookstore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Bookstore"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Bookstore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Bookstore.dir/build: Bookstore

.PHONY : CMakeFiles/Bookstore.dir/build

CMakeFiles/Bookstore.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Bookstore.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Bookstore.dir/clean

CMakeFiles/Bookstore.dir/depend:
	cd /mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/14908/Desktop/Bookstore /mnt/c/Users/14908/Desktop/Bookstore /mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug /mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug /mnt/c/Users/14908/Desktop/Bookstore/cmake-build-debug/CMakeFiles/Bookstore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Bookstore.dir/depend

