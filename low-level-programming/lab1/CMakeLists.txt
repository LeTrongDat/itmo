cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(DatabaseSystem VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Specify the source directory and the list of source files
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES 
    ${SOURCE_DIR}/db.c 
    ${SOURCE_DIR}/table.c 
    ${SOURCE_DIR}/utilities.c
)

# Create a library from source files
add_library(database_lib ${SOURCES})

# Add subdirectory for tests
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
