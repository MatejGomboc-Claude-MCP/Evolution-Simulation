cmake_minimum_required(VERSION 3.0)
project(evolution-sim-main)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the common library
include_directories(${CMAKE_SOURCE_DIR}/common)

# Create the executable
add_executable(evolution-sim main.cpp)

# Link with the common library
target_link_libraries(evolution-sim common)
