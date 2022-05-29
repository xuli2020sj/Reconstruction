cmake_minimum_required(VERSION 3.21)
project(Detection)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(tests)
add_subdirectory(util)

set(RELEASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/release)

include_directories(./3rdpart)

file(GLOB src_list ./src/*.cpp ./src/*.h )
add_executable(DECal ${src_list})

