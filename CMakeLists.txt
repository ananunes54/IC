cmake_minimum_required(VERSION 3.16)
project(simulation)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_executable(simulation simulation.c)

target_link_libraries(simulation PRIVATE SDL3::SDL3)
target_link_libraries(simulation PRIVATE m)
