project(Part1Tests LANGUAGES CXX)

add_executable(Part1Tests src/hash_test.cpp)

target_link_libraries(Part1Tests PRIVATE MyLib)

add_test(NAME Part1Tests COMMAND Part1Tests)