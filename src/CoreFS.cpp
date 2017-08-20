//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        CoreFS.cpp                                //
//            █ █        █ █        CoreFS                                    //
//             ████████████                                                   //
//           █              █       Copyright (c) 2017                        //
//          █     █    █     █      AmazingCow - www.AmazingCow.com           //
//          █     █    █     █                                                //
//           █              █       N2OMatt - n2omatt@amazingcow.com          //
//             ████████████         www.amazingcow.com/n2omatt                //
//                                                                            //
//                  This software is licensed as GPLv3                        //
//                 CHECK THE COPYING FILE TO MORE DETAILS                     //
//                                                                            //
//    Permission is granted to anyone to use this software for any purpose,   //
//   including commercial applications, and to alter it and redistribute it   //
//               freely, subject to the following restrictions:               //
//                                                                            //
//     0. You **CANNOT** change the type of the license.                      //
//     1. The origin of this software must not be misrepresented;             //
//        you must not claim that you wrote the original software.            //
//     2. If you use this software in a product, an acknowledgment in the     //
//        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
//        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
//        If you will not acknowledge, just send us a email. We'll be         //
//        *VERY* happy to see our work being used by other people. :)         //
//        The email is: acknowledgment_opensource@AmazingCow.com              //
//     3. Altered source versions must be plainly marked as such,             //
//        and must not be misrepresented as being the original software.      //
//     4. This notice may not be removed or altered from any source           //
//        distribution.                                                       //
//     5. Most important, you must have fun. ;)                               //
//                                                                            //
//      Visit opensource.amazingcow.com for more open-source projects.        //
//                                                                            //
//                                  Enjoy :)                                  //
//----------------------------------------------------------------------------//

//Header
#include "../include/CoreFS.h"
//C
#include <sys/types.h>
#include <sys/stat.h>
//std
#include <algorithm>
#include <cctype>
#include <sstream>
//CoreFS
#include "../include/private/Macros.h"

#if __linux__
    // Includes //
    #include <unistd.h>

#elif _WIN32
    // Includes //

    // Defines //
    //COWNOTE: On Windows, Microsoft did the favor to
    //  us to make the names differently.
    #define stat _stat
    #define getcwd _getcwd
#endif

//
//Notice:
//  Here will be defined the functions that are very easy to implement
//  in a multi platform way.
//  All functions that requires deep knowledge of the OS and/or are too
//  big will be defined on the OS's respective file.
//
//  For GNU/Linux - CoreFS_GNU_Linux.cpp
//  For Windows   - CoreFS_W32.cpp
//  For OSX       - CoreFS_OSX.cpp
//  ... and so on...
//
//  But please...
//  ADD ALL FUNCTIONS SIGNATURES HERE (and it's proper files)
//  COMMENTED OUT AND IN THE ORDER THAT IT IS DECLARED IN CoreFS.h.
//  This will enable search the files very easily since the structure
//  will be the same of every one of them.
//

////////////////////////////////////////////////////////////////////////////////
// Helpers Functions                                                          //
////////////////////////////////////////////////////////////////////////////////
bool check_stat_st_mode(const std::string &path, unsigned mask)
{
    struct stat sb = {0};
    if(stat(path.c_str(), &sb) != 0)
        return false;

    return (sb.st_mode & S_IFMT) == mask;
}


////////////////////////////////////////////////////////////////////////////////
// CoreFS API                                                                 //
////////////////////////////////////////////////////////////////////////////////
std::string CoreFS::GetPathSeparator()
{
#ifdef _WIN32
    return "\\";
#endif
    return "/";
}


////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
std::string CoreFS::CurrentDirectory()
{
    //COWTODO: Add an assertion on the pbuf value.
    auto p_buf = getcwd(nullptr, 0);
    auto cwd   = std::string(p_buf);

    SAFE_FREE(p_buf);

    return cwd;
}

std::string CoreFS::NewLine()
{
#ifdef _WIN32
    return "\r\n";
#endif
    return "\n";
}

std::string CoreFS::SystemDirectory()
{
    return GetFolderPath(CoreFS::SpecialFolder::System);
}

//  Defined in respective OS file.
//std::string GetFolderPath(SpecialFolder folder)

//  Defined in respective OS file.
//std::vector<std::string> GetLogicalDrives()


////////////////////////////////////////////////////////////////////////////////
// Python os.path Like API                                                    //
////////////////////////////////////////////////////////////////////////////////
//  Defined in respective OS file.
//std::string AbsPath(const std::string &path);

//Returns the final component of a pathname
std::string CoreFS::Basename(const std::string &path)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    return CoreFS::Split(path).second;
}

//Given a list of pathnames,
//returns the longest common leading component
std::string CoreFS::CommonPrefix(const std::initializer_list<std::string> &paths)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    if(paths.size() == 0)
        return "";

    auto s1 = std::min(paths);
    auto s2 = std::max(paths);

    for(int i = 0; i < s1.size(); ++i)
    {
        if(s1[i] != s2[i])
            return s1.substr(0, i);
    }

    return s1;
}

//Returns the directory component of a pathname
std::string CoreFS::Dirname(const std::string &path)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    return CoreFS::Split(path).first;
}

//Test whether a path exists.
//  Returns False for broken symbolic links
bool CoreFS::Exists(const std::string &path)
{
    struct stat sb = {0};
    return stat(path.c_str(), &sb) == 0;
}

//  Defined in respective OS file.
//std::string ExpandUser(const std::string &path);

//Return the last access time of a file, reported by os.stat().
time_t CoreFS::GetATime(const std::string &filename)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0)
        return -1;

    return sb.st_atime;
}

//Return the metadata change time of a file, reported by os.stat().
time_t GetCTime(const std::string &filename)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0)
        return -1;

    return sb.st_ctime;
}

//Return the last modification time of a file, reported by os.stat().
time_t GetMTime(const std::string &filename)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0)
        return -1;

    return sb.st_mtime;
}

//Return the size of a file, reported by os.stat().
long int GetSize(const std::string &filename)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py
    struct stat sb = {0};
    if(stat(filename.c_str(), &sb) != 0)
        return -1;

    return sb.st_size;
}

//  Defined in respective OS file.
//bool IsAbs(const std::string &path);

//Return true if the pathname refers to an existing directory.
bool CoreFS::IsDir(const std::string &path)
{
    return check_stat_st_mode(path, S_IFDIR);
}

//Test whether a path is a regular file
bool CoreFS::IsFile(const std::string &path)
{
    return check_stat_st_mode(path, S_IFREG);
}

//Test whether a path is a symbolic link.
//This will always return false for Windows prior to 6.0.
bool CoreFS::IsLink(const std::string &path)
{
    //COWTODO: Check a way to implement this easier and correctly.
    //  The windows sdk doesn't provides the S_IFLNK mask, but
    //  I found a value on the python's stat.py file located at:
    //      C:\Program Files\Anaconda3\Lib\stat.py
    //  I think that the value should be stable enought to use
    //  it here, since it, by its nature, can't change often.
    //  So make a research about it, and continue the implementation.
    return false;//check_stat_st_mode(path, S_IFLNK);
}

//COWTODO: Implement...
//bool IsMount(const std::string &path);

//Join two (or more) paths.
std::string CoreFS::Join(const std::vector<std::string> &paths)
{
    if(paths.size() == 0)
        return "";

    if(paths.size() == 1)
        return *paths.begin();

    return CoreFS::Join(
        *paths.begin(),
        std::vector<std::string>(paths.begin()+1, paths.end())
    );
}

//Join two (or more) paths.
std::string CoreFS::Join(
    const std::string &path,
    const std::vector<std::string> &paths)
{
    auto fullpath = path;
    auto sep      = GetPathSeparator();

    for(const auto &comp : paths)
    {
        //If the current component is just the path separator
        //but the fullpath is already filled we can skip it.
        //The only case that it's needed is the start of the
        //path since it indicates the root directory.
        if(comp.empty() || (comp == sep && !fullpath.empty()))
            continue;

        //COWTODO(n2omatt): There's any separators that are multi char?
        if(fullpath.back() == sep[0] && comp.front() == sep[0])
            fullpath += comp.substr(1);
        else if(fullpath.back() == sep[0] || comp.front() == sep[0])
            fullpath += comp;
        else
            fullpath += sep + comp;
    }

    return fullpath;
}


//COWTODO: Implement...
//bool LExists(const std::string &path);


//Normalize the case of a pathname.
//  On Unix and Mac OS X, this returns the path unchanged;
//  on case-insensitive filesystems, it converts the path to lowercase.
//  On Windows, it also converts forward slashes to backward slashes.
std::string CoreFS::NormCase(const std::string &path)
{
    //On non Windows platforms, just return the path.
    #ifndef _WIN32
    return path;
    #endif //ifndef _WIN32

    auto norm_path  = path;

    const auto &sep       = CoreFS::GetPathSeparator();
    const auto double_sep = sep + sep;

    //Replace the slashes.
    while(1)
    {
        auto pos = 0;

        pos = norm_path.find_first_of("/", pos);
        if(pos == std::string::npos)
            break;

        norm_path.replace(pos, 1, sep);
    }

    //Remove Duplicates.
    while(1)
    {
        auto pos = 0;

        pos = norm_path.find(double_sep, pos);
        if(pos == std::string::npos)
            break;

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

//Normalize path, eliminating double slashes, etc.
std::string CoreFS::NormPath(const std::string &path)
{
    //COWNOTE(n2omatt): Following the implementation of:
    //  /usr/lib/python2.7/posixpath.py
    if(path.empty())
        return ".";

    auto   sep             = GetPathSeparator();
    size_t initial_slashes = path[0] == sep[0] ? 1 : 0;

    if(initial_slashes != 0)
    {
        auto index = path.find_first_not_of(sep);
        initial_slashes = (index != std::string::npos) ? index : path.size();
    }

    auto components = std::vector<std::string>();
    auto curr_path  = path.substr(initial_slashes);

    while(!curr_path.empty())
    {
        auto index = curr_path.find_first_of(sep);
        auto comp  = curr_path.substr(0, index);

        auto double_dots         = (comp == "..");
        auto single_dot          = (comp ==  ".");
        auto comps_empty         = components.empty();
        auto already_double_dots = !comps_empty && components.back() == "..";
        auto start_with_slashes  = initial_slashes > 0;

        if(comp.empty())
        {
            //Do nothing...
        }
        //Same directory...
        else if(single_dot)
        {
            //Do nothing...
        }
        //Already on root of path...
        else if(double_dots && comps_empty && start_with_slashes)
        {
            //Do nothing...
        }
        //Go back one level...
        else if(double_dots && !comps_empty && !already_double_dots)
        {
            components.pop_back();
        }
        else
        {
            components.push_back(comp);
        }

        if(index == std::string::npos)
            break;

        curr_path = curr_path.substr(index +1);
    }


    //Make the output path.
    std::stringstream ss;

    //Put the initial slashes.
    //  POSIX allows one or two initial slashes, but treats
    //  three or more as single slash.
    initial_slashes = (initial_slashes > 2) ? 1 : initial_slashes;
    for(int i = 0; i < initial_slashes; ++i)
        ss << sep;
    //Put the components
    for(int i = 0; i < components.size(); ++i)
    {
        ss << components[i];
        if(i != components.size() -1)
            ss << sep;
    }

    return ss.str();
}

//  Defined in respective OS file.
//std::string RelPath(const std::string &path, const std::string &start = "");

//Test whether two pathnames reference the same actual file
bool CoreFS::SameFile(
    const std::string &filename1,
    const std::string &filename2)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/posixpath.py

    struct stat st1 = {0};
    struct stat st2 = {0};

    if(stat(filename1.c_str(), &st1) != 0)
        return false;

    if(stat(filename2.c_str(), &st2) != 0)
        return false;

    return st1.st_ino == st2.st_ino
        && st1.st_dev == st2.st_dev;
}

//COWNOTE: Not implemented
//sameopenfile(fp1, fp2)

//COWNOTE: Not implemented
//samestat(s1, s2)

//Split a pathname.
//  Return tuple (head, tail) where tail is everything after the final slash.
//  Either part may be empty.
std::pair<std::string, std::string> CoreFS::Split(const std::string &path)
{
    auto sep   = CoreFS::GetPathSeparator();
    auto index = path.rfind(sep);

    if(index == std::string::npos)
        return std::make_pair("", path);

    auto offset = (index == 0 && path[0] == sep[0]) ? 1 : 0;

    return std::make_pair(
        path.substr(0, index + offset),
        path.substr(index + 1)
    );
};

//Split a pathname.
//  Return a vector with all path components.
std::vector<std::string> CoreFS::SplitAll(const std::string &path)
{
    auto components = std::vector<std::string>();
    auto curr_path  = path;

    while(true)
    {
        auto comp_pair = CoreFS::Split(curr_path);

        if(!comp_pair.first.empty() && comp_pair.second.empty())
            components.push_back(comp_pair.first);
        else if(!comp_pair.second.empty())
            components.push_back(comp_pair.second);

        if(comp_pair.first == curr_path)
            break;

        curr_path = comp_pair.first;
    }

    return std::vector<std::string>(components.rbegin(), components.rend());
}

//COWNOTE: Not implemented
//splitdrive(p)

//Split the extension from a pathname.
//  Extension is everything from the last dot to the end, ignoring
//  leading dots.  Returns "(root, ext)"; ext may be empty.
std::pair<std::string, std::string> CoreFS::SplitExt(const std::string &path)
{
    //COWNOTE(n2omatt): Trying to follow:
    //  /usr/lib/python2.7/genericpath.py

    auto last_dot_index = path.rfind('.');
    //There's no dots on path
    //  So there's no extension separators.
    if(last_dot_index == std::string::npos)
        return std::make_pair(path, "");

    auto sep            = GetPathSeparator();
    auto last_sep_index = path.rfind(sep[0]);

    //The last dot happened not into final part of path.
    if(last_dot_index < last_sep_index &&
       last_sep_index != std::string::npos)
    {
        return std::make_pair(path, "");
    }

    auto filename_index = (int)last_dot_index - (int)last_sep_index -1;
    if(filename_index <= 0)
        return std::make_pair(path, "");

    auto base = path.substr(0, last_dot_index);
    auto ext  = path.substr(last_dot_index + 1);

    return std::make_pair(base, ext);
};

//COWNOTE: Not implemented.
//splitunc(p)
