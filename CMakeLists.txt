cmake_minimum_required (VERSION 2.6)

project (tp1)
 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread -O3")

add_executable(tp1 premiers.cpp)
