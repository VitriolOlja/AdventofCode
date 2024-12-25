# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and language
project(run VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)


# Add the executable
add_executable(run src/main.cpp src/Robot.cpp src/Direction.h src/Entity.h)

# Ensure the application is built as a console application
#target_link_options(run PRIVATE -mconsole)

