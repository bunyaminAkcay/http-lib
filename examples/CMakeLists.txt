cmake_minimum_required(VERSION 3.10)
project(http-lib)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../src ${CMAKE_BINARY_DIR}/src)

add_executable(example-server1 ../examples/example-server1.cc)
target_include_directories(example-server1 PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
target_link_libraries(example-server1 PRIVATE http-lib)

add_executable(example-server2 ../examples/example-server2.cc)
target_include_directories(example-server2 PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
target_link_libraries(example-server2 PRIVATE http-lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)