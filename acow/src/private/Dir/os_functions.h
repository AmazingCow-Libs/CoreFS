//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : os_functions.h                                                //
//  Project   : CoreDir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Function declarations for the platform specific pieces.                 //
//    To enable CoreDir.cpp to be implemented only once all functions         //
//    that are depended of platform specific pieces are forwarded to          //
//    functions declared here.                                                //
//                                                                            //
//    Each platform implement those functions separately.                     //
//---------------------------------------------------------------------------~//

#pragma once
// std
#include <vector>
#include <string>

namespace acow { namespace IO { namespace Dir { namespace _private {

//------------------------------------------------------------------------------
bool OS_Rename    (const std::string &src, const std::string &dst) noexcept;
bool OS_RemoveFile(const std::string &path                       ) noexcept;
bool OS_RemoveDir (const std::string &path                       ) noexcept;
bool OS_MKDir     (const std::string &path, int perms            ) noexcept;
void OS_CHDir     (const std::string &path                       ) noexcept;

//------------------------------------------------------------------------------
std::vector<std::string> OS_GetFilesystemEntries(
    const std::string &path,
    const std::string &pattern,
    bool               recursive,
    bool               getFiles,
    bool               getDirs) noexcept;

} // namespace _private
} // namespace Dir
} // namespace IO
} // namespace acow