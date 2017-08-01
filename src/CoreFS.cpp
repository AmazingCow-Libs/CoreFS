//COWTODO: Add license headers...
//Header
#include "../include/CoreFS.h"

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
//std::string CurrentDirectory()

std::string CoreFS::NewLine()
{
#ifdef _WIN32
    return "\r\n";
#endif
    return "\n";
}

//std::string SystemDirectory()
//std::string GetFolderPath(SpecialFolder folder)
//std::vector<std::string> GetLogicalDrives()


////////////////////////////////////////////////////////////////////////////////
// Python os.path Like API  Like API                                          //
////////////////////////////////////////////////////////////////////////////////
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
