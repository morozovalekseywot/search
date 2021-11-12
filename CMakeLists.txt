cmake_minimum_required(VERSION 3.20)
project(Mephi_Lab2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(libs/json11/include includes)

set(HEADERS
        libs/json11/include/json11.hpp
        includes/Index.hpp
        includes/Man.hpp
        includes/Multi_Set.hpp
        includes/Hash_table.hpp
        includes/lru_cache.hpp)

set(SOURCES
        libs/json11/src/json11.cpp)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES} ${HEADERS})
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
