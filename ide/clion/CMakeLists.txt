cmake_minimum_required(VERSION 3.8)
project(clion)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../include)
set(SOURCE_FILES
        ../../include/CoreFS.h
        ../../include/CoreFS_Utils.h
        ../../src/CoreFS.cpp
        main.cpp ../../src/CoreFS_GNU_Linux.cpp ../../include/private/Macros.h)
add_executable(clion ${SOURCE_FILES})