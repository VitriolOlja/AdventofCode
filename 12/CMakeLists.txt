# ...other code

include(CTest) # We include CTest which is part of CMake

add_subdirectory(Part1)

# We check if this is the main file
# you don't usually want users of your library to
# execute tests as part of their build
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(Part1Tests)
endif ()

enable_testing()