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
//  Project   : _private                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation for GNU/Linux.                                           //
//---------------------------------------------------------------------------~//

// AmazingCow Libs
#include "acow/cpp_goodies.h"

#if (ACOW_OS_IS_GNU_LINUX)

// Header
#include "os_functions.h"
// C
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
// std
#include <regex>
// acow_IO
#include "acow/include/Path.h"

// Usings
using namespace acow;
using namespace IO;
using namespace Dir;


//------------------------------------------------------------------------------
bool
_private::OS_Rename(const std::string &src, const std::string &dst) noexcept
{
    return rename(src.c_str(), dst.c_str()) == 0;
}

//------------------------------------------------------------------------------
bool
_private::OS_RemoveFile(const std::string &path) noexcept
{
    return remove(path.c_str()) == 0;
}

//------------------------------------------------------------------------------
bool
_private::OS_RemoveDir(const std::string &path) noexcept
{
    return rmdir(path.c_str()) == 0;
}

//------------------------------------------------------------------------------
bool
_private::OS_MKDir(const std::string &path, int perms) noexcept
{
    return mkdir(path.c_str(), perms) == 0;
}

//------------------------------------------------------------------------------
void
_private::os_chdir(const std::string &path) noexcept
{
    //COWTODO(n2omatt): Handle errors...
    chdir(path.c_str());
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
    std::vector<std::string> entries;

    auto *p_DIR = opendir(path.c_str());
    if(!p_DIR)
        return entries;

    auto re = std::regex(pattern, std::regex::egrep);
    auto sm = std::smatch();

    struct dirent *p_dirent = nullptr;
    while((p_dirent = readdir(p_DIR)) != nullptr) {
        auto name = std::string(p_dirent->d_name);
        if(name == ".." || name == ".") { continue; }

        // Test if name matches the pattern.
        auto match = std::regex_match(name, sm, re);
        if(!match) { continue; }

        auto fullname = CoreFS::Join(path, {name});
        bool is_dir   = CoreFS::IsDir(fullname);

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
    }

    return entries;
}

#endif // ACOW_OS_IS_GNU_LINUX
