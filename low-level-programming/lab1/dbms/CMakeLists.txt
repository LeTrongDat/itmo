# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project's name and version
project(DBMS VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files to the project executable
add_executable(DBMS
    main.cpp
    src/database.cpp
    # Add other .cpp files here
)

# Link with libraries (if required)
# target_link_libraries(DBMS <library_name>)
