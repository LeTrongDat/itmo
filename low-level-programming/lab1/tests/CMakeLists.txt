# Locate GTest for unit testing
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executables
add_executable(test_db test_db.c)
target_link_libraries(test_db GTest::GTest GTest::Main database_lib)

# Enable testing and add test
enable_testing()
add_test(NAME TestDatabase COMMAND test_db)
