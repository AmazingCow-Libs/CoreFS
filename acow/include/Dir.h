//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreDir.h                                                     //
//  Project   : CoreDir                                                       //
//  Date      : Aug 11, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//  This lib is a "rip-off" of the C# Directory class.                        //
//                                                                            //
//  We find very useful have those "functions" on C#,                         //
//  but we want to use them on C++ as well.                                   //
//  We made a lot of effort to make it correct and respect the semantics      //
//  of both original API and C++ usage.                                       //
//  But there some places that it wasn't possible or would make the API       //
//  hard to use. On these places we choose to the possibility                 //
//  of the easier usage.                                                      //
//                                                                            //
//  There some functions that aren't implemented and we don't have plans to   //
//  implement in a near future, for those, you're free to implement ;D        //
//                                                                            //
//  Lastly, our target here is make it very usable both on Windows and        //
//  GNU/Linux, there are no efforts currently to make it usable on OSX or     //
//  other Unices neither for mobile platforms.                                //
//  But this might change on future.                                          //
//                                                                            //
//  Have fun...                                                               //
//  n2omatt - <n2omatt@amazingcow.com>                                        //
//                                                                            //
//  C# API Reference:                                                         //
//    https://msdn.microsoft.com/en-us/library/system.io.directory.aspx       //
//---------------------------------------------------------------------------~//

#pragma once

// std
#include <string>
#include <vector>
// AmazingCow libs
#include "acow/cpp_goodies.h"
// acow_IO
#include "IO_Utils.h"

namespace acow { namespace IO { namespace Dir {

//------------------------------------------------------------------------------
// COWTODO(n2omatt: Functions to implement:
//   GetCreationTime
//   GetCreationTimeUtc
//   GetLastAccessTime
//   GetLastAccessTimeUtc
//   GetLastWriteTime
//   GetLastWriteTimeUtc
//   GetLogicalDrives
//   SetCreationTime
//   SetCreationTimeUtc
//   SetLastAccessTime
//   SetLastAccessTimeUtc
//   SetLastWriteTime
//   SetLastWriteTimeUtc


///-----------------------------------------------------------------------------
/// @brief
///   Creates all directories and subdirectories in the
///   specified path unless they already exist.
/// @param path
///   The path that directory will be created.
/// @param mode
///   The directories permissions (same as mkdir(1)).
///   This is ignored on Windows.
/// @returns
///   true if the operation succeeded, false otherwise.
/// @notes
///   CreateDirectory will try to undo (remove all created directories)
///   if at some point the operation fails. This will try to let the
///   filesystem at a consistent state.
bool CreateDirectory(const std::string &path, unsigned mode = 0777) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Deletes the specified directory and, if indicated, any
///   subdirectories and files in the directory.
/// @param path
///   The path of directory that will be removed.
/// @param recursive
///   If the function should remove underneath contents.
///   The behaviour is the same of rm(1) when recursive is set
///   to false (default) and rm(1) -rf when recursive is set to true.
/// @returns
///   true if the operation succeeded, false otherwise.
bool Delete(const std::string &path, bool recursive = false) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Determines whether the given path refers to
///   an existing directory on disk.
/// @param path
///   The path for the tested directory.
/// @returns
///   true if the entry exists **and** is a directory, false otherwise.
bool Exists(const std::string &path) noexcept;

///-----------------------------------------------------------------------------
//COWTODO(n2omatt): Doxygen...
std::string GetCurrentDirectory() noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns the names of the subdirectories (including their paths)
///   that match the specified search pattern in the specified directory,
///   and optionally searches subdirectories.
/// @param path
///   The start path of the search.
/// @param pattern
///   The regex that entries will be tested against.
///   The implementation pass the argument directly to std::regex_match,
///   so to futher details about it, please refer to:
///      http://en.cppreference.com/w/cpp/regex/regex_match
/// @param recursive
///   If the function should scan subdirectories.
/// @returns
///   The list of entries that match the pattern.
/// @see
///   GetFileSystemEntries, GetFiles.
std::vector<std::string> GetDirectories(
        const std::string &path,
        const std::string &pattern   = ".*",
        bool               recursive = true) noexcept;

///-----------------------------------------------------------------------------
//COWTODO(n2omatt): Doxygen...
std::string GetDirectoryRoot(const std::string &path) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns the names of files (including their paths) that match the
///   specified search pattern in the specified directory, using a value
///   to determine whether to search subdirectories.
/// @param path
///   The start path of the search.
/// @param pattern
///   The regex that entries will be tested against.
///   The implementation pass the argument directly to std::regex_match,
///   so to futher details about it, please refer to:
///      http://en.cppreference.com/w/cpp/regex/regex_match
/// @param recursive
///   If the function should scan subdirectories.
/// @returns
///   The list of entries that match the pattern.
/// @see
///   GetFileSystemEntries, GetDirectories.
std::vector<std::string> GetFiles(
    const std::string &path,
    const std::string &pattern   = ".*",
    bool               recursive = true) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns an array of all the file names and directory names that match
///   a search pattern in a specified path, and optionally
///   searches subdirectories.
/// @param path
///   The start path of the search.
/// @param pattern
///   The regex that entries will be tested against.
///   The implementation pass the argument directly to std::regex_match,
///   so to futher details about it, please refer to:
///      http://en.cppreference.com/w/cpp/regex/regex_match
/// @param recursive
///   If the function should scan subdirectories.
/// @returns
///   The list of entries that match the pattern.
/// @see
///   GetFiles, GetDirectories.
std::vector<std::string> GetFileSystemEntries(
    const std::string &path,
    const std::string &pattern   = ".*",
    bool               recursive = true) noexcept;

///-----------------------------------------------------------------------------
//COWTODO(n2omatt): Doxygen....
std::string GetParent(const std::string &path) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Moves a file or a directory and its contents to a new location.
/// @param src
///   Current location of the directory.
/// @param dst
///   Destination location of the directory.
/// @returns
///   true if the operation succeeded, false otherwise.
bool Move(const std::string &src, const std::string &dst) noexcept;

///-----------------------------------------------------------------------------
//COWTODO(n2omatt): Doxygen...
void SetCurrentDirectory(const std::string &path) noexcept;

} // namespace Dir
} // namespace IO
} // namespace acow
