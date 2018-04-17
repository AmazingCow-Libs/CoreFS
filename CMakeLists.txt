##~---------------------------------------------------------------------------##
##                     _______  _______  _______  _     _                     ##
##                    |   _   ||       ||       || | _ | |                    ##
##                    |  |_|  ||       ||   _   || || || |                    ##
##                    |       ||       ||  | |  ||       |                    ##
##                    |       ||      _||  |_|  ||       |                    ##
##                    |   _   ||     |_ |       ||   _   |                    ##
##                    |__| |__||_______||_______||__| |__|                    ##
##                             www.amazingcow.com                             ##
##  File      : CMakeLists.txt                                                ##
##  Project   : acow_cpp_goodies                                              ##
##  Date      : Feb 19, 2018                                                  ##
##  License   : GPLv3                                                         ##
##  Author    : n2omatt <n2omatt@amazingcow.com>                              ##
##  Copyright : AmazingCow - 2018                                             ##
##                                                                            ##
##  Description :                                                             ##
##                                                                            ##
##---------------------------------------------------------------------------~##

cmake_minimum_required(VERSION 3.5)


##----------------------------------------------------------------------------##
## Project                                                                    ##
##----------------------------------------------------------------------------##
set(PROJECT acow_IO)
project(${PROJECT})


##----------------------------------------------------------------------------##
## Sources                                                                    ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
## Sources
set(SOURCES         
    acow/src/Dir.cpp 
    acow/src/private/Dir/gnu_linux_functions.cpp
    acow/src/private/Dir/win32_functions.cpp

    acow/src/File.cpp
    
    acow/src/Path.cpp
    acow/src/private/Path/unix_functions.cpp
    acow/src/private/Path/win32_functions.cpp
)


##----------------------------------------------------------------------------##
## Library                                                                    ##
##----------------------------------------------------------------------------##
##------------------------------------------------------------------------------
## Lib.
add_library(${PROJECT} ${SOURCES})

##-----------------------------------------------------------------------------
## Include directories.
target_include_directories(${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

##------------------------------------------------------------------------------
## Dependencies.
target_link_libraries(${PROJECT} LINK_PUBLIC acow_c_goodies)
target_link_libraries(${PROJECT} LINK_PUBLIC acow_cpp_goodies)
