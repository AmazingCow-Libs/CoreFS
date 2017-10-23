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
///@brief
///  Gets the path separator for the platform.
std::string GetPathSeparator();


////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
///@brief
///  Gets the fully qualified path of the current working directory.
std::string CurrentDirectory();

///@brief
///  Gets the newline string defined for this environment.
std::string NewLine();

///@brief
///  Gets the fully qualified path of the system directory.
///  Same as GetFolderPath(SpecialFolder::System);
std::string SystemDirectory();

///@brief
///  Gets the path to the system special folder that is identified by the
///  specified enumeration.
///@note
///  While we tried to make the library much cross-platform as possible,
///  this function was ported from C# and with it brings all Windows'
///  specificities. This means that not all SpecialFolder values are
///  valid on non Windows platforms.
///  We had an option to "map" the missing values but since there's no obvious
///  way to do that besides pure guessing we found that is wise to let them
///  missing anyway.
///  This is exactly what [mono](https://github.com/mono/mono/blob/master/mcs/class/corlib/System/Environment.cs)
///  does.
///@returns
///  The fully qualified path for the given folder.
std::string GetFolderPath(SpecialFolder folder);


////////////////////////////////////////////////////////////////////////////////
// Python os.path Like API                                                    //
////////////////////////////////////////////////////////////////////////////////
///@brief
///  Return the absolute version of a path.
///@param path
///  The path that will be mapped.
std::string AbsPath(const std::string &path);

///@brief
///  Returns the final component of a pathname.
///@param path
///  The path that will be mapped.
std::string Basename(const std::string &path);

///@brief
///  Given a list of pathnames, returns the longest common leading component.
///@param paths
///  The paths that will be tested.
std::string CommonPrefix(const std::initializer_list<std::string> &paths);

///@brief
///  Returns the directory component of a pathname
///@param path
///  The path that will be mapped.
std::string Dirname(const std::string &path);

///@brief
///  Test whether a path exists.
///@param path
///  The path that will be tested.
///@note
///  Returns False for broken symbolic links
///@returns
///  True if path exists, false otherwise.
bool Exists(const std::string &path);

///@brief
///  Expand ~ and ~user constructs.
///  If user or $HOME is unknown, do nothing.
///@returns
///  The path with with the ~ and ~user constructs expanded
///  or the unmodified path if it cannot be expanded.
std::string ExpandUser(const std::string &path);

//COWTODO: Check if we gonna implement this.
//expandvars(const std::string &path)

///@brief
///  Return the last access time of a file, reported by os.stat().
///@param filename
///  The filename that will be tested.
time_t GetATime(const std::string &filename);

///@brief
///  Return the metadata change time of a file, reported by os.stat().
///@param filename
///  The filename that will be tested.
time_t GetCTime(const std::string &filename);

///@brief
///  Return the last modification time of a file, reported by os.stat().
///@param filename
///  The filename that will be tested.
time_t GetMTime(const std::string &filename);

///@brief
///  Return the size of a file, reported by os.stat().
///@param filename
///  The filename that will be tested.
long int GetSize(const std::string &filename);

///@brief
///  Test whether a path is absolute
///@param path
///  The path that will be tested.
bool IsAbs(const std::string &path);

///@brief
///   Return true if the pathname refers to an existing directory.
///@param path
///  The path that will be tested.
bool IsDir(const std::string &path);

///@brief
///  Test whether a path is a regular file
///@param path
///  The path that will be tested.
bool IsFile(const std::string &path);

///@brief
///  Test whether a path is a symbolic link.
///@param path
///  The path that will be tested.
///@note
///   This will always return false for Windows prior to 6.0.
///@warning NOT IMPLEMENTED YET!
bool IsLink(const std::string &path);

///@brief
///  Test whether a path is a mount point
///  (a drive root, the root of a share, or a mounted volume)
///@param path
///  The path that will be tested.
///@warning NOT IMPLEMENTED YET!
bool IsMount(const std::string &path);

///@brief
///  Join two (or more) paths.
///@param paths
///  A list of paths that will be joined.
std::string Join(const std::vector<std::string> &paths);

///@brief
///  Join two (or more) paths.
///@param path
///  The first part of path.
///@param paths
///  The rest of path components.
std::string Join(
    const std::string &path,
    const std::vector<std::string> &paths);

///@brief
///  Test whether a path exists.
///@note
///  Returns True for broken symbolic links
///@param path
///  The path that will be tested.
bool LExists(const std::string &path);

///@brief
///  Normalize the case of a pathname.
///  On Unix and Mac OS X, this returns the path unchanged.
///  On case-insensitive filesystems, it converts the path to lowercase.
///  On Windows, it also converts forward slashes to backward slashes.
///@param path
///  The path that will be normalized.
///@param forceForwardSlashes
///  On Windows makes the path use the '/' instead of '\'
std::string NormCase(const std::string &path, bool forceForwardSlashes = false);

///@brief
///  Normalize path, eliminating double slashes, etc.
///@param path
///  The path that will be normalized.
///@param forceForwardSlashes
///  On Windows makes the path use the '/' instead of '\'
std::string NormPath(const std::string &path, bool forceForwardSlashes = false);

///@brief
///  Return a relative version of a path
///@param path
///  The path that will be mapped
///@param start
///  The start point to map the relative path.
std::string RelPath(const std::string &path, const std::string &start = ".");

///@brief
///  Test whether two pathnames reference the same actual file
bool SameFile(const std::string &filename1, const std::string &filename2);

//COWNOTE: Not implemented
//sameopenfile(fp1, fp2)

//COWNOTE: Not implemented
//samestat(s1, s2)

///@brief
///  Split a pathname.
///@param path
///  The path that will be split.
///@returns
///  A tuple (head, tail) where tail is everything after the final slash.
///  Either part may be empty.
std::pair<std::string, std::string> Split(const std::string &path);

///@brief
///  Split a pathname.
///@param path
///  The path that will be split.
///@returns
///  A vector with all path components.
std::vector<std::string> SplitAll(const std::string &path);

//COWNOTE: Not implemented
//splitdrive(p)

///@brief
///  Split the extension from a pathname.
///  Extension is everything from the last dot to the end, ignoring
///  leading dots.
///@param path
///  The path that will be split.
///@returns
///  "(root, ext)"; ext may be empty.
std::pair<std::string, std::string> SplitExt(const std::string &path);

//COWNOTE: Not implemented.
//splitunc(p)

NS_COREFS_END
