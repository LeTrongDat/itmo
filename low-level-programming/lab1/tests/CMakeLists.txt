# Locate GTest for unit testing
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executables
add_executable(test_operations test_operations.c)
target_link_libraries(test_operations GTest::GTest GTest::Main database_lib)

# Enable testing and add test
enable_testing()
add_test(NAME TestOperations COMMAND test_operations)