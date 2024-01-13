cmake_minimum_required(VERSION 3.10)

# set the project name
project(statsmodeling)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the include directory for header files
include_directories(include)

# add implementation files
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(statsmodeling ${SOURCES})

# enable testing
enable_testing()
add_subdirectory(test)
