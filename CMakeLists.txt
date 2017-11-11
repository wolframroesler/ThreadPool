# Build configuration for multithreading test program
# by Wolfram Rösler <wolfram@roesler-ac.de> 2017-10-14

cmake_minimum_required (VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)
project (threaddemo CXX)
find_package (Threads REQUIRED)
add_executable (threaddemo threaddemo.cpp)
target_link_libraries (threaddemo Threads::Threads)
