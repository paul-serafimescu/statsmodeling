cmake_minimum_required(VERSION 3.10)

# Name the test project
project(statsmodeling_tests)

# Add the GoogleTest subdirectory
add_subdirectory(googletest)

# Include the GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define your test sources
set(TEST_SOURCES test_main.cpp test_OLS.cpp)

# Add the test executable
add_executable(runUnitTests ${TEST_SOURCES})

# Link the test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)

# Link the test executable against the project library if there is one
# If your project's source files are included directly in the main CMakeLists and not in a separate library,
# you would need to specify them here again or refactor your CMake to create a library target.
# target_link_libraries(runUnitTests your_project_library)

# Add the test definitions
add_test(NAME unitTests COMMAND runUnitTests)
