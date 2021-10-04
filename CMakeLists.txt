cmake_minimum_required(VERSION 3.20)
project(Mephi_Lab2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(Mephi_Lab2 main.cpp Index.hpp Man.hpp)