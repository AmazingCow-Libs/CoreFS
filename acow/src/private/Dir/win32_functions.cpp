//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : os_functions.cpp                                              //
//  Project   : CoreDir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation for Windows.                                             //
//---------------------------------------------------------------------------~//

// AmazingCow Libs
#include "acow/cpp_goodies.h"

#if (ACOW_OS_IS_WINDOWS)

// Header
#include "os_functions.h"
// std
#include <regex>
// Windows
#define WINDOWS_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
// acow_IO
#include "acow/include/IO_Utils.h"
#include "acow/include/Path.h"

// Usings
using namespace acow;
using namespace IO;
using namespace Dir;

//------------------------------------------------------------------------------
bool
_private::OS_Rename(const std::string &src, const std::string &dst) noexcept
{
    return MoveFileA(src.c_str(), dst.c_str());
}

//------------------------------------------------------------------------------
bool
_private::OS_RemoveFile(const std::string &path) noexcept
{
    return DeleteFileA(path.c_str());
}

//------------------------------------------------------------------------------
bool
_private::OS_RemoveDir(const std::string &path) noexcept
{
    return RemoveDirectoryA(path.c_str());
}

//------------------------------------------------------------------------------
bool
_private::OS_MKDir(
    const std::string &path,
    int /* perms - On Windows we ignore the flags... */) noexcept
{
    return CreateDirectoryA(path.c_str(), nullptr);
}

//------------------------------------------------------------------------------
void
_private::OS_CHDir(const std::string &path) noexcept
{
    // COWTODO(n2omatt): Bullet proof this...
    //   https://msdn.microsoft.com/en-us/library/windows/desktop/aa365530(v=vs.85).aspx
    SetCurrentDirectoryA(path.c_str());
}

//------------------------------------------------------------------------------
std::vector<std::string>
_private::OS_GetFilesystemEntries(
    const std::string &path,
    const std::string &pattern,
    bool               recursive,
    bool               getFiles,
    bool               getDirs) noexcept
{
    //--------------------------------------------------------------------------
    // References:
    //   https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx
    //   FindFirstFile: https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
    //   FindNextFile : https://msdn.microsoft.com/en-us/library/windows/desktop/aa364428(v=vs.85).aspx
    //   FindClose    : https://msdn.microsoft.com/en-us/library/windows/desktop/aa364413(v=vs.85).aspx
    std::vector<std::string> entries;

    WIN32_FIND_DATAA ffd         = { 0 };
    HANDLE           hFind       = INVALID_HANDLE_VALUE;
    auto             search_path = path + "\\*";

    hFind = FindFirstFileA(search_path.c_str(), &ffd);
    if(hFind == INVALID_HANDLE_VALUE) { return entries; }

    auto re = std::regex(pattern, std::regex::egrep);
    auto sm = std::smatch();

    do {
        auto name = std::string(ffd.cFileName);
        if(name == ".." || name == ".") { continue; }

        //Test if name matches the pattern.
        auto match = std::regex_match(name, sm, re);
        if(!match) { continue; }

        auto fullname = Path::Join(path, { name });
        bool is_dir   = Path::IsDir(fullname);

        if(is_dir && getDirs) {
            entries.push_back(fullname);
        } else if(!is_dir && getFiles) {
            entries.push_back(fullname);
        }

        if(recursive) {
            auto recursive_entries = OS_GetFilesystemEntries(
                fullname,
                pattern,
                recursive,
                getFiles,
                getDirs
            );

            std::copy(
                recursive_entries.begin(),
                recursive_entries.end(),
                std::back_inserter(entries)
            );
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    auto dwError = GetLastError();
    FindClose(hFind);

    if(dwError != ERROR_NO_MORE_FILES) { entries.clear(); }
    return entries;
}

#endif // ACOW_OS_IS_WINDOWS
