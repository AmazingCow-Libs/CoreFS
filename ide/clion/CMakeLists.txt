cmake_minimum_required(VERSION 3.8)
project(clion)

set(CMAKE_CXX_STANDARD 11)

include_directories(../../)

set(SOURCE_FILES
    ../../CoreFS/CoreFS.h
    ../../CoreFS/include/CoreFS.h
    ../../CoreFS/include/CoreFS_Utils.h
    ../../CoreFS/src/Macros.h
    ../../CoreFS/src/CoreFS.cpp
    ../../CoreFS/src/CoreFS_GNU_Linux.cpp
    ../../CoreFS/src/CoreFS_Win32.cpp
    main.cpp
)

add_executable(clion ${SOURCE_FILES})
