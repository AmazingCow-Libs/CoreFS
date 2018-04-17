//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : Path.cpp                                                    //
//  Project   : Path                                                        //
//  Date      : Aug 01, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017, 2018                                       //
//                                                                            //
//  Description :                                                             //
//    Implementation of functions that are general enough to be               //
//    implemented for multiple OSes                                           //
//                                                                            //
//    All other functions resides on:                                         //
//      For GNU/Linux - Path_GNU_Linux.cpp                                  //
//      For Windows   - Path_W32.cpp                                        //
//      For OSX       - Path_OSX.cpp                                        //
//      ... and so on...                                                      //
//                                                                            //
//    But please...                                                           //
//    ADD ALL FUNCTIONS SIGNATURES HERE (and it's proper files)               //
//    COMMENTED OUT AND IN THE ORDER THAT IT IS DECLARED IN Path.h.         //
//    This will enable search the files very easily since the structure       //
//    will be the same of every one of them.                                  //
//                                                                            //
//---------------------------------------------------------------------------~//

// Header
#include "acow/include/Path.h"
// C
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
// std
#include <algorithm>
#include <cctype>
#include <sstream>
// Amazing Cow Libs
#include "acow/cpp_goodies.h"
// acow_IO - Path Private
#include "private/Path/os_functions.h"

//------------------------------------------------------------------------------
// Platform dependent includes and defines.
#if (ACOW_OS_IS_UNIX)
    //--------------------------------------------------------------------------
    // Includes
    #include <unistd.h>

#elif (ACOW_OS_IS_WINDOWS)
    //--------------------------------------------------------------------------
    // Includes
    #include <direct.h>

    //--------------------------------------------------------------------------
    // Defines
    // COWNOTE: On Windows, Microsoft did the favor to
    //          us to make the names differently.
    #define stat   _stat
    #define getcwd _getcwd

    #undef GetTempFileName
    #undef GetTempPath
    #undef min
    #undef max
#endif // Path_IS_UNIX

// Usings
using namespace acow;
using namespace IO;



//------------------------------------------------------------------------------
// Notice:
//   Here will be defined the functions that are very easy to implement
//   in a multi platform way.
//   All functions that requires deep knowledge of the OS and/or are too
//   big will be defined on the OS's respective file.
//
//   For GNU/Linux - Path_GNU_Linux.cpp
//   For Windows   - Path_W32.cpp
//   For OSX       - Path_OSX.cpp
//   ... and so on...
//
//   But please...
//   ADD ALL FUNCTIONS SIGNATURES HERE (and it's proper files)
//   COMMENTED OUT AND IN THE ORDER THAT IT IS DECLARED IN Path.h.
//   This will enable search the files very easily since the structure
//   will be the same of every one of them.
//

//----------------------------------------------------------------------------//
// Helpers Functions                                                          //
//----------------------------------------------------------------------------//
acow_internal_function bool
check_stat_st_mode(const std::string &path, unsigned mask) noexcept
{
    struct stat sb = {0};
    if(stat(path.c_str(), &sb) != 0)
        return false;

    return (sb.st_mode & S_IFMT) == mask;
}


//----------------------------------------------------------------------------//
// Path API                                                                 //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string
Path::GetPathSeparator() noexcept
{
#if (ACOW_OS_IS_WINDOWS)
    return "\\";
#endif
    return "/";
}

std::string
Path::ExpandUserAndMakeAbs(const std::string &path) noexcept
{
    return Path::AbsPath(Path::ExpandUser(path));
}


//----------------------------------------------------------------------------//
// C# System.Path Like API                                                    //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string
Path::ChangeExtension(
    const std::string &path,
    const std::string &newExt) noexcept
{
    if(path.empty()) { return path; }

    auto split = Path::SplitExt(path);

    if(newExt.empty()  ) { return split.first;                } // Remove extension.
    if(newExt[0] != '.') { return split.first + "." + newExt; } // Add extension with dot.

    return split.first + newExt; // Add extension (already with dot)
}

//------------------------------------------------------------------------------
std::string
Path::GetExtension(const std::string &path) noexcept
{
    return Path::SplitExt(path).second;
}

//------------------------------------------------------------------------------
/// @brief
///   Returns a random folder name or file name.
std::string
Path::GetRandomFileName() noexcept
{
    ACOW_NOT_IMPLEMENTED();
    return "";
}

//------------------------------------------------------------------------------
std::string
Path::GetTempFileName() noexcept
{
    char buffer[L_tmpnam];
    auto result = tmpnam(buffer);

    ACOW_ASSERT(result != nullptr, "Failed to generate TempFileName.");
    if(result == nullptr) { // Cannot generate the filename.
        return "";
    }

    FILE* p_file = fopen(buffer, "rw+");
    ACOW_ASSERT(p_file, "Failed to open TempFileName - Filename: (%s)", buffer);
    if(!p_file)  { // Cannot open the file with generated filename.
        return "";
    }

    // COWTODO(n2omatt): We shouldn't close the file???
    return std::string(buffer);
}

//------------------------------------------------------------------------------
std::string
Path::GetTempPath() noexcept
{
    return _private::OS_GetTempPath();
}

//------------------------------------------------------------------------------
bool
Path::HasExtension(const std::string &path) noexcept
{
    return !Path::SplitExt(path).second.empty();
}


//----------------------------------------------------------------------------//
// C# System.Environment Like API                                             //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string
Path::CurrentDirectory() noexcept
{
    // COWTODO: Add an assertion on the pbuf value.
    auto p_buf = getcwd(nullptr, 0);
    auto cwd   = std::string(p_buf);

    ACOW_SAFE_FREE(p_buf);
    return cwd;
}

//------------------------------------------------------------------------------
std::string
Path::NewLine() noexcept
{
#if (ACOW_OS_IS_WINDOWS)
    return "\r\n";
#endif
    return "\n";
}

//------------------------------------------------------------------------------
std::string
Path::SystemDirectory() noexcept
{
    return GetFolderPath(Path::SpecialFolder::System);
}

//------------------------------------------------------------------------------
std::string
Path::GetFolderPath(SpecialFolder folder) noexcept
{
    return _private::OS_GetFolderPath(folder);
}


//----------------------------------------------------------------------------//
// Python os.path Like API                                                    //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string
Path::AbsPath(const std::string &path) noexcept
{
    return _private::OS_GetAbsPath(path);
}

//------------------------------------------------------------------------------
std::string
Path::Basename(const std::string &path) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    return Path::Split(path).second;
}

//------------------------------------------------------------------------------
std::string
Path::CommonPrefix(const std::initializer_list<std::string> &paths) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    if(paths.size() == 0)
        return "";

    auto s1 = std::min(paths);
    auto s2 = std::max(paths);

    for(size_t i = 0; i < s1.size(); ++i) {
        if(s1[i] != s2[i]) {
            return s1.substr(0, i);
        }
    }

    return s1;
}

//------------------------------------------------------------------------------
std::string
Path::Dirname(const std::string &path) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    return Path::Split(path).first;
}

//------------------------------------------------------------------------------
bool
Path::Exists(const std::string &path) noexcept
{
    struct stat sb = {0};
    return stat(path.c_str(), &sb) == 0;
}

//------------------------------------------------------------------------------
std::string
Path::ExpandUser(const std::string &path) noexcept
{
    //COWNOTE(n2omatt): Following the python's os.path.expand
    //  user located at: /usr/lib/python2.7/posixpath.py
    if(path.size() == 0 || path[0] != '~') { return path; }

    std::string home_path;
    auto norm_path         = Path::NormCase(path, true);
    auto first_slash_index = norm_path.find(Path::GetPathSeparator());

    // There's no slashes on path.
    if(first_slash_index == std::string::npos) {
        first_slash_index = path.size();
    }

    // Path is in format of: ~/something/very/nice
    if(first_slash_index == 1) {
        home_path = _private::OS_GetUserHome();
    }
    // Path is in format of: ~some_user/something/very/nice
    else {
        auto username = path.substr(1, first_slash_index);
        home_path     = _private::OS_GetUserHome(username);
    }

    return Path::Join(
        home_path,
        {path.substr(first_slash_index, path.size() - first_slash_index)}
    );
}

//------------------------------------------------------------------------------
time_t
Path::GetATime(const std::string &filename) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0) { return -1; }
    return sb.st_atime;
}

//------------------------------------------------------------------------------
time_t
Path::GetCTime(const std::string &filename) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0) { return -1; }
    return sb.st_ctime;
}

//------------------------------------------------------------------------------
time_t
Path::GetMTime(const std::string &filename) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0) { return -1; }
    return sb.st_mtime;
}

//------------------------------------------------------------------------------
long int
Path::GetSize(const std::string &filename) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0) { return -1; }
    return sb.st_size;
}

//------------------------------------------------------------------------------
bool
Path::IsAbs(const std::string &path) noexcept
{
    return _private::OS_IsAbs(path);
}

//------------------------------------------------------------------------------
bool
Path::IsDir(const std::string &path) noexcept
{
    return check_stat_st_mode(path, S_IFDIR);
}

//------------------------------------------------------------------------------
bool
Path::IsFile(const std::string &path) noexcept
{
    return check_stat_st_mode(path, S_IFREG);
}

//------------------------------------------------------------------------------
bool
Path::IsLink(const std::string &path) noexcept
{
    //COWTODO: Check a way to implement this easier and correctly.
    //  The windows sdk doesn't provides the S_IFLNK mask, but
    //  I found a value on the python's stat.py file located at:
    //      C:\Program Files\Anaconda3\Lib\stat.py
    //  I think that the value should be stable enought to use
    //  it here, since it, by its nature, can't change often.
    //  So make a research about it, and continue the implementation.
    ACOW_NOT_IMPLEMENTED();
    return false;// check_stat_st_mode(path, S_IFLNK);
}

//------------------------------------------------------------------------------
bool
Path::IsMount(const std::string &path) noexcept
{
    return _private::OS_IsMount(path);
}

//------------------------------------------------------------------------------
std::string
Path::Join(const std::vector<std::string> &paths) noexcept
{
    if(paths.size() == 0) { return "";            }
    if(paths.size() == 1) { return paths.front(); }

    return Path::Join(
        *paths.begin(),
        std::vector<std::string>(paths.begin()+1, paths.end())
    );
}

//------------------------------------------------------------------------------
std::string
Path::Join(
    const std::string &path,
    const std::vector<std::string> &paths) noexcept
{
    auto fullpath = path;
    auto sep      = GetPathSeparator();

    for(const auto &comp : paths) {
        //If the current component is just the path separator
        //but the fullpath is already filled we can skip it.
        //The only case that it's needed is the start of the
        //path since it indicates the root directory.
        if(comp.empty() || (comp == sep && !fullpath.empty())) { continue; }

        //COWTODO(n2omatt): There's any separators that are multi char?
        if(fullpath.back() == sep[0] && comp.front() == sep[0]) {
            fullpath += comp.substr(1);
        } else if(fullpath.back() == sep[0] || comp.front() == sep[0]) {
            fullpath += comp;
        } else {
            fullpath += sep + comp;
        }
    }

    return fullpath;
}

//------------------------------------------------------------------------------
//COWTODO: Implement...
//bool LExists(const std::string &path);

//------------------------------------------------------------------------------
std::string
Path::NormCase(
    const std::string &path,
    bool               forceForwardSlashes /* = false */) noexcept
{
    // On non Windows platforms, just return the path.
    #if (!ACOW_OS_IS_WINDOWS)
        return path;
    #endif // #if (!ACOW_OS_IS_WINDOWS)

    auto       norm_path  = path;
    const auto &sep       = Path::GetPathSeparator();
    const auto double_sep = sep + sep;

    //Replace the slashes.
    while(1) {
        auto pos = 0;

        pos = norm_path.find_first_of("/", pos);
        if(pos == std::string::npos) { break; }
        norm_path.replace(pos, 1, sep);
    }

    //Remove Duplicates.
    while(1) {
        auto pos = 0;

        pos = norm_path.find(double_sep, pos);
        if(pos == std::string::npos) { break; }
        norm_path.replace(pos, 1, "");
    }

    //Make lowercase.
    std::transform(
        norm_path.begin(),
        norm_path.end  (),
        norm_path.begin(),
        ::tolower
    );

    return norm_path;
}

//------------------------------------------------------------------------------
std::string
Path::NormPath(
    const std::string &path,
    bool               forceForwardSlashes /* = false */) noexcept
{
    //COWNOTE(n2omatt): Following the implementation of:
    //  /usr/lib/python2.7/posixpath.py
    if(path.empty()) { return "."; }

    auto   sep             = Path::GetPathSeparator();
    auto   replace_sep     = (forceForwardSlashes) ? "/" : sep;
    size_t initial_slashes = path[0] == sep[0] ? 1 : 0;

    if(initial_slashes != 0) {
        auto index = path.find_first_not_of(sep);
        initial_slashes = (index != std::string::npos) ? index : path.size();
    }

    auto components = std::vector<std::string>();
    auto curr_path  = path.substr(initial_slashes);

    while(!curr_path.empty()) {
        auto index = curr_path.find_first_of(sep);
        auto comp  = curr_path.substr(0, index);

        auto double_dots         = (comp == "..");
        auto single_dot          = (comp ==  ".");
        auto comps_empty         = components.empty();
        auto already_double_dots = !comps_empty && components.back() == "..";
        auto start_with_slashes  = initial_slashes > 0;

        if(comp.empty()) {
            //Do nothing...
        }
        //Same directory...
        else if(single_dot) {
            //Do nothing...
        }
        //Already on root of path...
        else if(double_dots && comps_empty && start_with_slashes) {
            //Do nothing...
        }
        //Go back one level...
        else if(double_dots && !comps_empty && !already_double_dots) {
            components.pop_back();
        }
        else {
            components.push_back(comp);
        }

        if(index == std::string::npos) { break; }
        curr_path = curr_path.substr(index +1);
    }


    //Make the output path.
    std::stringstream ss;

    //Put the initial slashes.
    //  POSIX allows one or two initial slashes, but treats
    //  three or more as single slash.
    initial_slashes = (initial_slashes > 2) ? 1 : initial_slashes;
    for(size_t i = 0; i < initial_slashes; ++i) {
        ss << replace_sep;
    }

    //Put the components
    for(size_t i = 0; i < components.size(); ++i) {
        ss << components[i];
        if(i != components.size() -1) { ss << replace_sep; }
    }

    return ss.str();
}

//------------------------------------------------------------------------------
std::string
Path::RelPath(
    const std::string &path, 
    const std::string &start /* = "" */) noexcept
{
    return Path::_private::OS_RelPath(path, start);
}

//------------------------------------------------------------------------------
bool
Path::SameFile(
    const std::string &filename1,
    const std::string &filename2) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py

    struct stat st1 = {0};
    struct stat st2 = {0};

    if(stat(filename1.c_str(), &st1) != 0) { return false; }
    if(stat(filename2.c_str(), &st2) != 0) { return false; }

    return st1.st_ino == st2.st_ino
        && st1.st_dev == st2.st_dev;
}

//------------------------------------------------------------------------------
//COWNOTE: Not implemented
//sameopenfile(fp1, fp2)

//------------------------------------------------------------------------------
//COWNOTE: Not implemented
//samestat(s1, s2)

//------------------------------------------------------------------------------
std::pair<std::string, std::string>
Path::Split(const std::string &path) noexcept
{
    auto sep   = Path::GetPathSeparator();
    auto index = path.rfind(sep);

    if(index == std::string::npos) {
        return std::make_pair("", path);
    }

    auto offset = (index == 0 && path[0] == sep[0]) ? 1 : 0;

    return std::make_pair(
        path.substr(0, index + offset),
        path.substr(index + 1)
    );
};

//------------------------------------------------------------------------------
std::vector<std::string>
Path::SplitAll(const std::string &path) noexcept
{
    auto components = std::vector<std::string>();
    auto curr_path  = path;

    while(true) {
        auto comp_pair = Path::Split(curr_path);

        if(!comp_pair.first.empty() && comp_pair.second.empty()) {
            components.push_back(comp_pair.first);
        } else if(!comp_pair.second.empty()) {
            components.push_back(comp_pair.second);
        }

        if(comp_pair.first == curr_path) { break; }
        curr_path = comp_pair.first;
    }

    return std::vector<std::string>(components.rbegin(), components.rend());
}

//------------------------------------------------------------------------------
//COWNOTE: Not implemented
//splitdrive(p)

//------------------------------------------------------------------------------
std::pair<std::string, std::string>
Path::SplitExt(const std::string &path) noexcept
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/genericpath.py

    auto last_dot_index = path.rfind('.');
    //There's no dots on path
    //  So there's no extension separators.
    if(last_dot_index == std::string::npos) {
        return std::make_pair(path, "");
    }

    auto sep            = GetPathSeparator();
    auto last_sep_index = path.rfind(sep[0]);

    //The last dot happened not into final part of path.
    if(last_dot_index < last_sep_index &&
       last_sep_index != std::string::npos)
    {
        return std::make_pair(path, "");
    }

    auto filename_index = (int)last_dot_index - (int)last_sep_index -1;
    if(filename_index <= 0) {
        return std::make_pair(path, "");
    }

    auto base = path.substr(0, last_dot_index);
    auto ext  = path.substr(last_dot_index + 1);

    return std::make_pair(base, ext);
};

//------------------------------------------------------------------------------
//COWNOTE: Not implemented.
//splitunc(p)
