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
CMAKE_SOURCE_DIR = /Users/datle/Projects/itmo/low-level-programming/lab1-v2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/datle/Projects/itmo/low-level-programming/lab1-v2/build

# Include any dependencies generated for this target.
include CMakeFiles/database_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/database_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/database_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/database_lib.dir/flags.make

CMakeFiles/database_lib.dir/src/db.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/db.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/db.c
CMakeFiles/database_lib.dir/src/db.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/database_lib.dir/src/db.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/db.c.o -MF CMakeFiles/database_lib.dir/src/db.c.o.d -o CMakeFiles/database_lib.dir/src/db.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/db.c

CMakeFiles/database_lib.dir/src/db.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/db.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/db.c > CMakeFiles/database_lib.dir/src/db.c.i

CMakeFiles/database_lib.dir/src/db.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/db.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/db.c -o CMakeFiles/database_lib.dir/src/db.c.s

CMakeFiles/database_lib.dir/src/table.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/table.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/table.c
CMakeFiles/database_lib.dir/src/table.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/database_lib.dir/src/table.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/table.c.o -MF CMakeFiles/database_lib.dir/src/table.c.o.d -o CMakeFiles/database_lib.dir/src/table.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/table.c

CMakeFiles/database_lib.dir/src/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/table.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/table.c > CMakeFiles/database_lib.dir/src/table.c.i

CMakeFiles/database_lib.dir/src/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/table.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/table.c -o CMakeFiles/database_lib.dir/src/table.c.s

CMakeFiles/database_lib.dir/src/row.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/row.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/row.c
CMakeFiles/database_lib.dir/src/row.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/database_lib.dir/src/row.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/row.c.o -MF CMakeFiles/database_lib.dir/src/row.c.o.d -o CMakeFiles/database_lib.dir/src/row.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/row.c

CMakeFiles/database_lib.dir/src/row.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/row.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/row.c > CMakeFiles/database_lib.dir/src/row.c.i

CMakeFiles/database_lib.dir/src/row.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/row.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/row.c -o CMakeFiles/database_lib.dir/src/row.c.s

CMakeFiles/database_lib.dir/src/column.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/column.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/column.c
CMakeFiles/database_lib.dir/src/column.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/database_lib.dir/src/column.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/column.c.o -MF CMakeFiles/database_lib.dir/src/column.c.o.d -o CMakeFiles/database_lib.dir/src/column.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/column.c

CMakeFiles/database_lib.dir/src/column.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/column.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/column.c > CMakeFiles/database_lib.dir/src/column.c.i

CMakeFiles/database_lib.dir/src/column.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/column.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/column.c -o CMakeFiles/database_lib.dir/src/column.c.s

CMakeFiles/database_lib.dir/src/data.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/data.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/data.c
CMakeFiles/database_lib.dir/src/data.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/database_lib.dir/src/data.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/data.c.o -MF CMakeFiles/database_lib.dir/src/data.c.o.d -o CMakeFiles/database_lib.dir/src/data.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/data.c

CMakeFiles/database_lib.dir/src/data.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/data.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/data.c > CMakeFiles/database_lib.dir/src/data.c.i

CMakeFiles/database_lib.dir/src/data.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/data.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/data.c -o CMakeFiles/database_lib.dir/src/data.c.s

CMakeFiles/database_lib.dir/src/operations.c.o: CMakeFiles/database_lib.dir/flags.make
CMakeFiles/database_lib.dir/src/operations.c.o: /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/operations.c
CMakeFiles/database_lib.dir/src/operations.c.o: CMakeFiles/database_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/database_lib.dir/src/operations.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/database_lib.dir/src/operations.c.o -MF CMakeFiles/database_lib.dir/src/operations.c.o.d -o CMakeFiles/database_lib.dir/src/operations.c.o -c /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/operations.c

CMakeFiles/database_lib.dir/src/operations.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/database_lib.dir/src/operations.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/operations.c > CMakeFiles/database_lib.dir/src/operations.c.i

CMakeFiles/database_lib.dir/src/operations.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/database_lib.dir/src/operations.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/datle/Projects/itmo/low-level-programming/lab1-v2/src/operations.c -o CMakeFiles/database_lib.dir/src/operations.c.s

# Object files for target database_lib
database_lib_OBJECTS = \
"CMakeFiles/database_lib.dir/src/db.c.o" \
"CMakeFiles/database_lib.dir/src/table.c.o" \
"CMakeFiles/database_lib.dir/src/row.c.o" \
"CMakeFiles/database_lib.dir/src/column.c.o" \
"CMakeFiles/database_lib.dir/src/data.c.o" \
"CMakeFiles/database_lib.dir/src/operations.c.o"

# External object files for target database_lib
database_lib_EXTERNAL_OBJECTS =

libdatabase_lib.a: CMakeFiles/database_lib.dir/src/db.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/src/table.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/src/row.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/src/column.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/src/data.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/src/operations.c.o
libdatabase_lib.a: CMakeFiles/database_lib.dir/build.make
libdatabase_lib.a: CMakeFiles/database_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C static library libdatabase_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/database_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/database_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/database_lib.dir/build: libdatabase_lib.a
.PHONY : CMakeFiles/database_lib.dir/build

CMakeFiles/database_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/database_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/database_lib.dir/clean

CMakeFiles/database_lib.dir/depend:
	cd /Users/datle/Projects/itmo/low-level-programming/lab1-v2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/datle/Projects/itmo/low-level-programming/lab1-v2 /Users/datle/Projects/itmo/low-level-programming/lab1-v2 /Users/datle/Projects/itmo/low-level-programming/lab1-v2/build /Users/datle/Projects/itmo/low-level-programming/lab1-v2/build /Users/datle/Projects/itmo/low-level-programming/lab1-v2/build/CMakeFiles/database_lib.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/database_lib.dir/depend
