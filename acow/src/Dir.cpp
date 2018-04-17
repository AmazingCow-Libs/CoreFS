//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : Dir.cpp                                                   //
//  Project   : Dir                                                       //
//  Date      : Oct 17, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation for the Dir functions.                               //
//    Notice that this file implements all functions in a platform agnostic   //
//    way, so all things that are specific to each platform is forwarded      //
//    to a external function.                                                 //
//                                                                            //
//    This functions are defined in a "private" header, called os_functions.h //
//    and implemented separately for each supported platform.                 //
//                                                                            //
//    So to add a new platform is just create the platform directory          //
//    and implement the os_functions.cpp for it.                              //
//---------------------------------------------------------------------------~//

// Header
#include "acow/include/Dir.h"
// C
#include <stdio.h>
// acow_IO
#include "acow/include/IO_Utils.h"
#include "acow/include/Path.h"
#include "private/Dir/os_functions.h"

// Usings
using namespace acow;
using namespace IO;

//----------------------------------------------------------------------------//
// Function Definitions                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
bool
Dir::CreateDirectory(
    const std::string &path,
    unsigned           mode /* = 0777 */) noexcept
{
    if(Path::Exists(path)) { return false; }

    auto curr_path          = Path::AbsPath(path);
    auto components         = Path::SplitAll(curr_path);
    auto dirs_created       = std::vector<std::string>();
    auto clear_dirs_created = false; // For when an error occurs
                                     // we can delete the created dirs.
    curr_path.clear();
    curr_path = components.front();

    for(auto it = ++components.begin(); it != components.end(); ++it) {
        curr_path        = Path::Join(curr_path, {*it});
        auto need_create = !Path::Exists(curr_path);

        if(need_create) {
            //Some error occurred...
            //  Let's try to delete all created directories.
            if(!Dir::_private::OS_MKDir(curr_path.c_str(), mode)) {
                clear_dirs_created = true;
                break;
            }

            dirs_created.push_back(curr_path);
        }
    }

    //An error occurred that prevented the fullpath to be created
    //  So we gonna try to delete all directories that we created
    //  before, this way we try to maintain the filesystem consistency
    //  as we found before the functions.
    if(clear_dirs_created) {
        for(auto rit = dirs_created.rbegin();
            rit != dirs_created.rend();
            ++rit) {
            Dir::_private::OS_RemoveDir(*rit);
        }
        //COWTODO(n2omatt): How we gonna handle errors???
    }

    return !clear_dirs_created;
}

//------------------------------------------------------------------------------
bool
Dir::Delete(const std::string &path, bool recursive /* = false */) noexcept
{
    auto all_files = GetFiles(path, ".*", true);
    //We can't delete non empty directories.
    if(all_files.size() > 0 && !recursive) { return false; }

    auto all_dirs = GetDirectories(path, ".*", true);

    //Remove all files
    //  This will let all underneath directories empty...
    for(const auto &filename : all_files) {
        auto success = Dir::_private::OS_RemoveFile(filename);
        if(!success) { return false; }
    }

    //Remove all directories.
    for(const auto &dirname : all_dirs) {
        auto success = Dir::_private::OS_RemoveFile(dirname);
        if(!success) { return false; }
    }

    Dir::_private::OS_RemoveDir(path);
    return true;
}


//------------------------------------------------------------------------------
bool
Dir::Exists(const std::string &path) noexcept
{
    return Path::IsDir(path);
}

//------------------------------------------------------------------------------
std::string
Dir::GetCurrentDirectory() noexcept
{
    return Path::CurrentDirectory();
}

//------------------------------------------------------------------------------
std::vector<std::string>
Dir::GetDirectories(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */ ) noexcept
{
    return Dir::_private::OS_GetFilesystemEntries(
        Path::AbsPath(path),
        pattern,
        recursive,
        false,
        true
    );
}

//------------------------------------------------------------------------------
std::string
Dir::GetDirectoryRoot(const std::string &path) noexcept
{
    auto fullpath = (Path::IsAbs(path))
        ? path
        : Path::Join(Path::CurrentDirectory(), {path});

    auto components = Path::SplitAll(fullpath);
    return components.front();
}

//------------------------------------------------------------------------------
std::vector<std::string>
Dir::GetFiles(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */) noexcept
{
    return Dir::_private::OS_GetFilesystemEntries(
        Path::AbsPath(path),
        pattern,
        recursive,
        true,
        false
    );
}

//------------------------------------------------------------------------------
std::vector<std::string>
Dir::GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   /* = ".*"  */,
    bool               recursive /* = true */) noexcept
{
    return Dir::_private::OS_GetFilesystemEntries(
        Path::AbsPath(path),
        pattern,
        recursive,
        true,
        true
    );
}


//------------------------------------------------------------------------------
std::string
Dir::GetParent(const std::string &path) noexcept
{
    if(Path::IsAbs(path)) { return path; }
    return Path::AbsPath(Path::Join(path, {".."}));
}

//------------------------------------------------------------------------------
bool
Dir::Move(const std::string &src, const std::string &dst) noexcept
{
    auto abs_src = Path::AbsPath(src);
    auto abs_dst = Path::AbsPath(dst);

    if( abs_src == abs_dst  ) { return false; }
    if(!Dir::Exists(abs_src)) { return false; }
    if( Dir::Exists(abs_dst)) { return false; }

    return Dir::_private::OS_Rename(abs_src.c_str(), abs_dst.c_str());
}


///-----------------------------------------------------------------------------
void
Dir::SetCurrentDirectory(const std::string &path) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors...
    auto fullpath = Path::AbsPath(path);
    Dir::_private::OS_CHDir(fullpath);
}
