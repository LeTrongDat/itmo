# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/datle/Projects/itmo/low-level-programming/lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/datle/Projects/itmo/low-level-programming/lab1/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_operations.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_operations.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_operations.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_operations.dir/flags.make

tests/CMakeFiles/test_operations.dir/test_operations.c.o: tests/CMakeFiles/test_operations.dir/flags.make
tests/CMakeFiles/test_operations.dir/test_operations.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1/tests/test_operations.c
tests/CMakeFiles/test_operations.dir/test_operations.c.o: tests/CMakeFiles/test_operations.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test_operations.dir/test_operations.c.o"
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/test_operations.dir/test_operations.c.o -MF CMakeFiles/test_operations.dir/test_operations.c.o.d -o CMakeFiles/test_operations.dir/test_operations.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1/tests/test_operations.c

tests/CMakeFiles/test_operations.dir/test_operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_operations.dir/test_operations.c.i"
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1/tests/test_operations.c > CMakeFiles/test_operations.dir/test_operations.c.i

tests/CMakeFiles/test_operations.dir/test_operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_operations.dir/test_operations.c.s"
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1/tests/test_operations.c -o CMakeFiles/test_operations.dir/test_operations.c.s

# Object files for target test_operations
test_operations_OBJECTS = \
"CMakeFiles/test_operations.dir/test_operations.c.o"

# External object files for target test_operations
test_operations_EXTERNAL_OBJECTS =

tests/test_operations: tests/CMakeFiles/test_operations.dir/test_operations.c.o
tests/test_operations: tests/CMakeFiles/test_operations.dir/build.make
tests/test_operations: libdatabase_lib.a
tests/test_operations: /opt/homebrew/lib/libgtest_main.a
tests/test_operations: /opt/homebrew/lib/libgtest.a
tests/test_operations: tests/CMakeFiles/test_operations.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_operations"
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_operations.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_operations.dir/build: tests/test_operations
.PHONY : tests/CMakeFiles/test_operations.dir/build

tests/CMakeFiles/test_operations.dir/clean:
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_operations.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_operations.dir/clean

tests/CMakeFiles/test_operations.dir/depend:
	cd /Users/datle/Projects/itmo/low-level-programming/lab1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/datle/Projects/itmo/low-level-programming/lab1 /Users/datle/Projects/itmo/low-level-programming/lab1/tests /Users/datle/Projects/itmo/low-level-programming/lab1/build /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests /Users/datle/Projects/itmo/low-level-programming/lab1/build/tests/CMakeFiles/test_operations.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/test_operations.dir/depend

