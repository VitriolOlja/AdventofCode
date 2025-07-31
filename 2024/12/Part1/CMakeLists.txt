cmake_minimum_required(VERSION 3.15)

project(run VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

#find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
#if(CMAKE_CXX_CPPCHECK)
#	list(APPEND CMAKE_CXX_CPPCHECK "--enable=all")
#endif()

add_executable(run src/main.cpp)

# include(CTest)