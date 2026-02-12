cmake_minimum_required(VERSION 3.16)
project(simulation)

add_library(utils STATIC r8lib.c c8lib.c)
target_include_directories(utils PRIVATE include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_executable(simulation simulation.c matrix_exponential.c)
target_include_directories(simulation PRIVATE include)

target_link_libraries(simulation PRIVATE SDL3::SDL3)
target_link_libraries(simulation PRIVATE m)
target_link_libraries(simulation PRIVATE utils)
