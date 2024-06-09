cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Documents VERSION 1.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add the source files
set(SOURCES
    NoCopyVector.cpp
)

# Add the header files (optional, but useful for IDEs)
set(HEADERS
    NoCopyVector.hpp
)

# Create the library (static or shared)
add_library(MyZCVector STATIC ${SOURCES} ${HEADERS})

