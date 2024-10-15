cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(SimpleWebServer VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(webserver main.cpp)

# Include directories (if you have any)
target_include_directories(webserver PRIVATE include)

# Optionally link any required libraries
# target_link_libraries(webserver some_library)
