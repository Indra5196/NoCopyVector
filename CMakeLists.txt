cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the current source directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# List the header files and the template implementation files
set(HEADERS
    NoCopyVector.hpp
)

set(SOURCES
    NoCopyVector.tpp
)

# Create the library (static)
add_library(MyZCVector STATIC ${HEADERS} ${SOURCES})

# Add an executable to test the library
add_executable(TestProgram main.cpp)
target_link_libraries(TestProgram MyZCVector)

set_target_properties(MyZCVector PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(MyZCVector PRIVATE -Wall -Wextra -pedantic)
target_compile_options(TestProgram PRIVATE -Wall -Wextra -pedantic)

# Print target properties for diagnostics
get_target_property(LINKER_LANGUAGE MyZCVector LINKER_LANGUAGE)
message(STATUS "MyZCVector Linker Language: ${LINKER_LANGUAGE}")