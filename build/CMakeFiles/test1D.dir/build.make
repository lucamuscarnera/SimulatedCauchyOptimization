# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /u/sw/toolchains/gcc-glibc/11.2.0/base/bin/cmake

# The command to remove a file.
RM = /u/sw/toolchains/gcc-glibc/11.2.0/base/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jellyfish/shared-folder/MACSProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jellyfish/shared-folder/MACSProject/build

# Include any dependencies generated for this target.
include CMakeFiles/test1D.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test1D.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test1D.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test1D.dir/flags.make

CMakeFiles/test1D.dir/test1D.cpp.o: CMakeFiles/test1D.dir/flags.make
CMakeFiles/test1D.dir/test1D.cpp.o: ../test1D.cpp
CMakeFiles/test1D.dir/test1D.cpp.o: CMakeFiles/test1D.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jellyfish/shared-folder/MACSProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test1D.dir/test1D.cpp.o"
	/u/sw/toolchains/gcc-glibc/11.2.0/prefix/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test1D.dir/test1D.cpp.o -MF CMakeFiles/test1D.dir/test1D.cpp.o.d -o CMakeFiles/test1D.dir/test1D.cpp.o -c /home/jellyfish/shared-folder/MACSProject/test1D.cpp

CMakeFiles/test1D.dir/test1D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test1D.dir/test1D.cpp.i"
	/u/sw/toolchains/gcc-glibc/11.2.0/prefix/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jellyfish/shared-folder/MACSProject/test1D.cpp > CMakeFiles/test1D.dir/test1D.cpp.i

CMakeFiles/test1D.dir/test1D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test1D.dir/test1D.cpp.s"
	/u/sw/toolchains/gcc-glibc/11.2.0/prefix/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jellyfish/shared-folder/MACSProject/test1D.cpp -o CMakeFiles/test1D.dir/test1D.cpp.s

# Object files for target test1D
test1D_OBJECTS = \
"CMakeFiles/test1D.dir/test1D.cpp.o"

# External object files for target test1D
test1D_EXTERNAL_OBJECTS =

test1D: CMakeFiles/test1D.dir/test1D.cpp.o
test1D: CMakeFiles/test1D.dir/build.make
test1D: CMakeFiles/test1D.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jellyfish/shared-folder/MACSProject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test1D"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test1D.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test1D.dir/build: test1D
.PHONY : CMakeFiles/test1D.dir/build

CMakeFiles/test1D.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test1D.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test1D.dir/clean

CMakeFiles/test1D.dir/depend:
	cd /home/jellyfish/shared-folder/MACSProject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jellyfish/shared-folder/MACSProject /home/jellyfish/shared-folder/MACSProject /home/jellyfish/shared-folder/MACSProject/build /home/jellyfish/shared-folder/MACSProject/build /home/jellyfish/shared-folder/MACSProject/build/CMakeFiles/test1D.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test1D.dir/depend
