# Recommended minimum version for modern practices
cmake_minimum_required(VERSION 3.10) 
project(MyProject C)

# Set the C standard and enforce it (essential for cross-platform consistency)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF) # Better portability

# --- 1. Define Source Files Explicitly ---
# It's better practice to list source files manually rather than using GLOB_RECURSE.
# GLOB_RECURSE makes dependency tracking unreliable.

set(KD_TREE_SOURCES
    src/kd_tree/kd_tree.c
)

set(HEAP_SOURCES
    src/heap/heap.c
    src/heap/min_heap.c
)

set(MAIN_SOURCES
    src/main.c
)

# Combine all source files
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${HEAP_SOURCES}
    ${KD_TREE_SOURCES}
)

# --- 2. Create Executable and Link Components ---
add_executable(myprog ${ALL_SOURCES})

# Use target_include_directories for better encapsulation and dependency management.
# This makes the include directory available only to the 'myprog' target.
# 'PUBLIC' means it's needed for compiling this target AND for any other target that links to it (if this were a library).
target_include_directories(myprog 
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# --- 3. Post-Build Step for Data (Improved) ---
# Ensure the custom command runs AFTER the executable is successfully created.
# We also use the target name for the output executable to make it robust.
add_custom_command(TARGET myprog POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:myprog>/data
    COMMENT "Copying data directory to the executable location."
)