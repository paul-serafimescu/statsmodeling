cmake_minimum_required(VERSION 3.10)

# Name the test project
project(statsmodeling_tests)

# include parent headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# include parent source
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# include python
include_directories(${Python_INCLUDE_DIRS})

file(GLOB PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")
add_library(statsmodeling_lib STATIC ${PROJECT_SOURCES})

# Add the GoogleTest subdirectory
add_subdirectory(googletest)

# Include the GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define your test sources
set(TEST_SOURCES test_OLS.cpp test_ACF.cpp test_PACF.cpp)

# Add the test executable
add_executable(runUnitTests ${TEST_SOURCES})

# Link the test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main statsmodeling_lib)

# Link the test executable against the project library if there is one
# If your project's source files are included directly in the main CMakeLists and not in a separate library,
# you would need to specify them here again or refactor your CMake to create a library target.

target_link_libraries(runUnitTests ${Python_LIBRARIES})

# Add the test definitions
add_test(NAME unitTests COMMAND runUnitTests)
