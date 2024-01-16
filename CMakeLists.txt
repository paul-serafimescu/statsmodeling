cmake_minimum_required(VERSION 3.20)

# set the project name
project(statsmodeling)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-g")

include_directories(include)

# add implementation files
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(statsmodeling ${SOURCES})

find_package(Python COMPONENTS Development.Embed NumPy REQUIRED)

include(CMakePrintHelpers)
cmake_print_variables(Python_INCLUDE_DIRS)
cmake_print_variables(Python_NumPy_INCLUDE_DIRS)
cmake_print_variables(Python_NumPy_FOUND)
cmake_print_variables(Python_Interpreter_FOUND)
cmake_print_variables(Python_LIBRARIES)

include_directories(${Python_INCLUDE_DIRS} ${Python_NumPy_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} Python::Python Python::Module Python::NumPy)

# enable testing
enable_testing()
add_subdirectory(test)
