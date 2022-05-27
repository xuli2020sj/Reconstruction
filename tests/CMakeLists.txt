cmake_minimum_required(VERSION 3.20)
project(cpp)
include_directories(.)

file(GLOB SRC_LIST *.cpp)


foreach(SRC ${SRC_LIST})
    get_filename_component(file ${SRC} NAME_WLE)
    add_executable(${file} ${SRC})
    message(${file})
endforeach()