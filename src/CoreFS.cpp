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


#if _WIN32
    //COWNOTE: On Windows, Microsoft did the favor to us to make the
    //  name differently.
    #define stat _stat
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
    struct stat sb;
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
//  Defined in repective OS file.
//std::string CurrentDirectory()

std::string CoreFS::NewLine()
{
#ifdef _WIN32
    return "\r\n";
#endif
    return "\n";
}

//  Defined in repective OS file.
//std::string SystemDirectory()

//  Defined in repective OS file.
//std::string GetFolderPath(SpecialFolder folder)

//  Defined in repective OS file.
//std::vector<std::string> GetLogicalDrives()


////////////////////////////////////////////////////////////////////////////////
// Python os.path Like API  Like API                                          //
////////////////////////////////////////////////////////////////////////////////

//COWTODO: Implement...
//std::string AbsPath(const std::string &path);

//COWTODO: Implement...
//std::string Basename(const std::string &path);

//COWTODO: Implement...
//std::string CommonPath(const std::initializer_list<std::string> &paths);

//COWTODO: Implement...
//std::string CommonPrefix(const std::initializer_list<std::string> &paths);

//COWTODO: Implement...
//std::string Dirname(const std::string &path);

//COWTODO: Implement...
bool CoreFS::Exists(const std::string &path)
{
    struct stat sb;
    return stat(path.c_str(), &sb) == 0;
}

//COWTODO: Implement...
//std::string ExpandUser(const std::string &path);

//COWTODO: Implement...
//unsigned long GetATime(const std::string &filename);

//COWTODO: Implement...
//unsigned long GetCTime(const std::string &filename);

//COWTODO: Implement...
//unsigned long GetMTime(const std::string &filename);

//COWTODO: Implement...
//unsigned long GetSize(const std::string &filename);

//COWTODO: Implement...
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
std::string CoreFS::Join(
    const std::string &path,
    const std::initializer_list<std::string> &paths)
{
    auto fullpath = path;
    auto sep      = GetPathSeparator();
    auto sep_size = sep.size();

    for(const auto &comp : paths)
    {
        if(std::equal(fullpath.end() - sep_size, fullpath.end(), sep.begin()))
            fullpath += comp;
        else
            fullpath += sep + comp;
    }

    return fullpath;
}


//COWTODO: Implement...
//bool LExists(const std::string &path);

//COWTODO: Implement...
//std::string NormCase(const std::string &path);

//Normalize the case of a pathname.
//  On Unix and Mac OS X, this returns the path unchanged;
//  on case-insensitive filesystems, it converts the path to lowercase.
//  On Windows, it also converts forward slashes to backward slashes.
std::string CoreFS::NormPath(const std::string &path)
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
        std::tolower
    );

    return norm_path;
}

//COWTODO: Implement...
//std::string AbsPath(const std::string &path);

//COWTODO: Implement...
//std::string RelPath(const std::string &path, const std::string &start = "");

//COWTODO: Implement...
//bool SameFile(const std::string &filename1, const std::string &filename2);

//COWNOTE: Not implemented
//sameopenfile(fp1, fp2)

//COWNOTE: Not implemented
//samestat(s1, s2)

//COWTODO: Implement...
//std::pair<std::string, std::string> Split(const std::string &path);

//COWNOTE: Not implemented
//splitdrive(p)

//COWTODO: Implement...
//std::pair<std::string, std::string> SplitExt(const std::string &path);

//COWNOTE: Not implemented.
//splitunc(p)
