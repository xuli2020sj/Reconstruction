cmake_minimum_required(VERSION 3.20)
project(util)

include_directories(.)
include_directories(../3rdpart)


file(GLOB SRC_LIST *.cpp)



foreach(SRC ${SRC_LIST})
    get_filename_component(file ${SRC} NAME_WLE)
    add_executable(${file} ${SRC})
    message(${file})
endforeach()