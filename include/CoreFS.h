//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFS.h                                                      //
//  Project   : CoreFS                                                        //
//  Date      : Aug 01, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    This lib is a "rip-off" of the C# Environment class and                 //
//    python's os.path module.                                                //
//                                                                            //
//    We find very useful have those "functions" on C# and python,            //
//    but we want to use them on C++ as well.                                 //
//    We made a lot of effort to make it correct and respect the              //
//    semantics of both original API and C++ usage.                           //
//    But there some places that it wasn't possible or would make             //
//    the API hard to use. On these places we choose to the possibility       //
//    of the easier usage.                                                    //
//                                                                            //
//    There some functions that aren't implemented and we don't have          //
//    plans to implement in a near future, for those, you're free             //
//    to implement ;D                                                         //
//                                                                            //
//    On the C# API we tried to match as much as possible the usage           //
//    of the Windows SDK, there are SEVERAL values for                        //
//    CoreFS::SpecialFolder that aren't defined for Unix-like OSes            //
//    Just like it's on the .net side too.                                    //
//                                                                            //
//    Today we're matching the Mono implementation of those values,           //
//    but maybe if we find a easier and correct way, we might add "hacks"     //
//    for values that aren't defined in mono.                                 //
//                                                                            //
//    Lastly, our target here is make it very usable both on Windows and      //
//    GNU/Linux,there are no efforts currently to make it usable on OSX       //
//    or other Unices neither for mobile platforms.                           //
//    But this might change on future.                                        //
//                                                                            //
//    Have fun...                                                             //
//    n2omatt - <n2omatt@amazingcow.com>                                      //
//                                                                            //
//    C# API Reference:                                                       //
//      https://msdn.microsoft.com/en-us/library/system.environment.aspx      //
//    Python API Reference:                                                   //
//       https://docs.python.org/2/library/os.path.html                       //
//                                                                            //
//---------------------------------------------------------------------------~//

#pragma once
//std
#include <string>
#include <vector>
//CoreFS
#include "CoreFS_Utils.h"

NS_COREFS_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Enums / Constants / Typedefs                                               //
////////////////////////////////////////////////////////////////////////////////
enum class SpecialFolder {
    AdminTools,
    ApplicationData,
    CDBurning,
    CommonAdminTools,
    CommonApplicationData,
    CommonDesktopDirectory,
    CommonDocuments,
    CommonMusic,
    CommonOemLinks,
    CommonPictures,
    CommonProgramFiles,
    CommonProgramFilesX86,
    CommonPrograms,
    CommonStartMenu,
    CommonStartup,
    CommonTemplates,
    CommonVideos,
    Cookies,
    Desktop,
    DesktopDirectory,
    Favorites,
    Fonts,
    History,
    InternetCache,
    LocalApplicationData,
    LocalizedResources,
    MyComputer,
    MyDocuments,
    MyMusic,
    MyPictures,
    MyVideos,
    NetworkShortcuts,
    Personal,
    PrinterShortcuts,
    ProgramFiles,
    ProgramFilesX86,
    Programs,
    Recent,
    Resources,
    SendTo,
    StartMenu,
    Startup,
    System,
    SystemX86,
    Templates,
    UserProfile,
    Windows
};


////////////////////////////////////////////////////////////////////////////////
// CoreFS API                                                                 //
////////////////////////////////////////////////////////////////////////////////
std::string GetPathSeparator();


////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
std::string CurrentDirectory();
std::string NewLine();
std::string SystemDirectory();

std::string GetFolderPath(SpecialFolder folder);
std::vector<std::string> GetLogicalDrives();


////////////////////////////////////////////////////////////////////////////////
// Python os.path Like API                                                    //
////////////////////////////////////////////////////////////////////////////////
//Return the absolute version of a path.
std::string AbsPath(const std::string &path);

//Returns the final component of a pathname
std::string Basename(const std::string &path);

//Given a list of pathnames,
//returns the longest common leading component
std::string CommonPrefix(const std::initializer_list<std::string> &paths);

//Returns the directory component of a pathname
std::string Dirname(const std::string &path);

//Test whether a path exists.
//  Returns False for broken symbolic links
bool Exists(const std::string &path);

//Expand ~ and ~user constructs.
//  If user or $HOME is unknown, do nothing.
std::string ExpandUser(const std::string &path);

//COWTODO: Check if we gonna implement this.
//expandvars(const std::string &path)

//Return the last access time of a file, reported by os.stat().
time_t GetATime(const std::string &filename);

//Return the metadata change time of a file, reported by os.stat().
time_t GetCTime(const std::string &filename);

//Return the last modification time of a file, reported by os.stat().
time_t GetMTime(const std::string &filename);

//Return the size of a file, reported by os.stat().
long int GetSize(const std::string &filename);

//Test whether a path is absolute
bool IsAbs(const std::string &path);

//Return true if the pathname refers to an existing directory.
bool IsDir(const std::string &path);

//Test whether a path is a regular file
bool IsFile(const std::string &path);

//Test whether a path is a symbolic link.
//This will always return false for Windows prior to 6.0.
bool IsLink(const std::string &path);

//Test whether a path is a mount point (a drive root, the root of a
//share, or a mounted volume)
bool IsMount(const std::string &path);

//Join two (or more) paths.
std::string Join(const std::vector<std::string> &paths);

//Join two (or more) paths.
std::string Join(
    const std::string &path,
    const std::vector<std::string> &paths);

//Test whether a path exists.
//  Returns True for broken symbolic links
bool LExists(const std::string &path);

//Normalize the case of a pathname.
//  On Unix and Mac OS X, this returns the path unchanged;
//  on case-insensitive filesystems, it converts the path to lowercase.
//  On Windows, it also converts forward slashes to backward slashes.
std::string NormCase(const std::string &path, bool forceForwardSlashes = false);

//Normalize path, eliminating double slashes, etc.
std::string NormPath(const std::string &path, bool forceForwardSlashes = false);

//Return a relative version of a path
std::string RelPath(const std::string &path, const std::string &start = ".");

//Test whether two pathnames reference the same actual file
bool SameFile(const std::string &filename1, const std::string &filename2);

//COWNOTE: Not implemented
//sameopenfile(fp1, fp2)

//COWNOTE: Not implemented
//samestat(s1, s2)

//Split a pathname.
//  Return tuple (head, tail) where tail is everything after the final slash.
//  Either part may be empty.
std::pair<std::string, std::string> Split(const std::string &path);

//Split a pathname.
//  Return a vector with all path components.
std::vector<std::string> SplitAll(const std::string &path);

//COWNOTE: Not implemented
//splitdrive(p)

//Split the extension from a pathname.
//  Extension is everything from the last dot to the end, ignoring
//  leading dots.  Returns "(root, ext)"; ext may be empty.
std::pair<std::string, std::string> SplitExt(const std::string &path);

//COWNOTE: Not implemented.
//splitunc(p)

NS_COREFS_END
