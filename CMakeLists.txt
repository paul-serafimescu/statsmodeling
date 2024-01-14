cmake_minimum_required(VERSION 3.20)

# set the project name
project(statsmodeling)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-g")

# add the include directory for header files
include_directories(include)

# add implementation files
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(statsmodeling ${SOURCES})

# enable testing
enable_testing()
add_subdirectory(test)
