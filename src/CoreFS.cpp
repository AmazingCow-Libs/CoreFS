//COWTODO: Add license headers...
//Header
#include "../include/CoreFS.h"
//std
#include <sys/types.h>
#include <sys/stat.h>

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

//COWTODO: Implement...
//std::string NormPath(const std::string &path);

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
