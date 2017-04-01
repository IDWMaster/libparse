cmake_minimum_required(VERSION 3.4.1)
# This is a header-only library and does not require compilation.
add_executable(tester main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")