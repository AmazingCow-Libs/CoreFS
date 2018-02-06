
add_library(CoreFS
    CoreFS/src/CoreFS.cpp
    CoreFS/src/CoreFS_Unix.cpp
    CoreFS/src/CoreFS_Win32.cpp
)

target_include_directories(CoreFS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
