//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFile.h                                                    //
//  Project   : CoreFile                                                      //
//  Date      : Aug 26, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#pragma once

// std
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
// AmazingCow Libs
#include "acow/cpp_goodies.h"
// acow_IO
#include "IO_Utils.h"

namespace acow { namespace IO { namespace File {

///-----------------------------------------------------------------------------
/// @brief The mode that file will be opened.
namespace FileMode
{
    namespace Text {
        constexpr auto kRead  = "r";
        constexpr auto kWrite = "w";

        constexpr auto kReadWrite_Open     = "r+";
        constexpr auto kReadWrite_Truncate = "w+";

        constexpr auto kAppend          = "a";
        constexpr auto kAppend_Truncate = "a+";
    }

    namespace Binary {
        constexpr auto kRead  = "rb";
        constexpr auto kWrite = "wb";

        constexpr auto kReadWrite_Open     = "r+b";
        constexpr auto kReadWrite_Truncate = "w+b";

        constexpr auto kAppend          = "ab";
        constexpr auto kAppend_Truncate = "a+b";
    }
} // namespace FileMode


//----------------------------------------------------------------------------//
// Append                                                                     //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Appends lines to a file, and then closes the file.
///   If the specified file does not exist, this method creates a file,
///   writes the specified lines to the file, and then closes the file.
/// @param filename
///   Filename where the lines will be written.
/// @param lines
///   The content that will be written at file.
void AppendAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Opens a file, appends the specified string to the file,
///   and then closes the file.
///   If the file does not exist, this method creates a file, writes
///   the specified string to the file, then closes the file.
/// @param filename
///   Filename where the content will be written.
/// @param contents
///   The content that will be written.
void AppendAllText(
    const std::string &filename,
    const std::string &contents) noexcept;


//----------------------------------------------------------------------------//
// Copy                                                                       //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Copies an existing file to a new file.
///   Overwriting a file of the same name is allowed.
/// @param src
///   The source file.
/// @param dst
///   The destination file.
/// @param overwrite
///   If true destination will be overwritten if it already exists.
void Copy(
    const std::string &src,
    const std::string &dst,
    bool              overwrite = false) noexcept;


//----------------------------------------------------------------------------//
// Create                                                                     //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Creates or overwrites a file in the specified path.
/// @param filename
///   The name of file that will be created.
/// @note
///   This function will create the file in binary mode.
/// @return A std::fstream object.
std::unique_ptr<std::fstream> Create(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Creates or overwrites a file in the specified path.
/// @param filename
///   The name of file that will be created.
/// @note
///   This function will create the file in text mode.
/// @return A std::fstream object.
std::unique_ptr<std::fstream> CreateText(const std::string &filename) noexcept;


//----------------------------------------------------------------------------//
// Delete                                                                     //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Deletes the specified file.
/// @param filename
///   The file name that will be deleted.
void Delete(const std::string &filename) noexcept;


//----------------------------------------------------------------------------//
// Exists                                                                     //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief Check if file exists.
bool Exists(const std::string &filename) noexcept;

//COWTODO: Gonna implement that???
//GetAttributes(const std::string &filename) ???


//----------------------------------------------------------------------------//
// Get * Time                                                                 //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Returns the creation date and time of the specified file or directory.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetCreationTime(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns the creation date and time, in coordinated universal time (UTC),
///   of the specified file or directory.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetCreationTimeUtc(const std::string &filename) noexcept;


///-----------------------------------------------------------------------------
/// @brief
///   Returns the date and time the specified file or directory
///   was last accessed.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetLastAccessTime(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns the date and time, in coordinated universal time (UTC),
///   that the specified file or directory was last accessed.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetLastAccessTimeUtc(const std::string &filename) noexcept;


///-----------------------------------------------------------------------------
/// @brief
///   Returns the date and time the specified file or directory
///   was last written to.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetLastWriteTime(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Returns the date and time, in coordinated universal time (UTC),
///   that the specified file or directory was last written to.
/// @param filename
///   The name of file that will be queried.
/// @returns
///   A tm_t object with the info about the file.
tm_t GetLastWriteTimeUtc(const std::string &filename) noexcept;


//----------------------------------------------------------------------------//
// Move                                                                       //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Moves a specified file to a new location, providing the option to
///   specify a new file name.
/// @param src
///   The source filename.
/// @param dst
///   The destination filename.
/// @param overwrite
///   If true destination will be overwritten if it already exists.
void Move(
    const std::string &src,
    const std::string &dst,
    bool               overwrite = false) noexcept;


//----------------------------------------------------------------------------//
// Open                                                                       //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Opens a std::fstream on the specified path with read/write access.
/// @param filename
///   The name of file that will be opened.
/// @param filemode
///   The desired file mode to open the file.
/// @returns A std::fstream object.
/// @see FileMode
std::unique_ptr<std::fstream> Open(
    const std::string &filename,
    const std::string &filemode) noexcept;

//COWTODO(n2omatt): Add an Open override with std::ios_base::openmode filemodes.


///-----------------------------------------------------------------------------
/// @brief
///   Opens a std::fstream on the specified path with read/write access.
/// @param filename
///   The name of file that will be opened.
/// @note
///   The file will be open binary mode (FileMode::Binary::kRead).
/// @returns A std::fstream object.
std::unique_ptr<std::fstream> OpenRead(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Opens an existing text file for reading.
/// @param filename
///   The name of file that will be opened.
/// @note
///   The file will be open text mode (FileMode::Text::kRead).
/// @returns A std::fstream object.
std::unique_ptr<std::fstream> OpenText(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Opens an existing file or creates a new file for writing.
/// @param filename
///   The name of file that will be opened.
/// @note
///   The file will be open binary mode (FileMode::Binary::kRead).
/// @returns A std::fstream object.
std::unique_ptr<std::fstream> OpenWrite(const std::string &filename) noexcept;


//----------------------------------------------------------------------------//
// Read                                                                       //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Opens a binary file, reads the contents of the file into a byte array,
///   and then closes the file.
/// @param filename
///   The name of tile that will be read.
/// @returns
///   A vector of bytes.
/// @see byte_t.
std::vector<u8> ReadAllBytes(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Opens a text file, reads all lines of the file, and then closes the file.
/// @param filename
///   The name of tile that will be read.
/// @returns
///   The file lines.
std::vector<std::string> ReadAllLines(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Opens a text file, reads all lines of the file, and then closes the file.
/// @param filename
///   The name of tile that will be read.
/// @returns
///   The entire file contents into a string.
std::string ReadAllText(const std::string &filename) noexcept;


//COWTODO(n2omatt): Check how to implement that....
//Replace(const std::string &filename, const std::string &filename, const std::string &filename, Boolean) ???


//----------------------------------------------------------------------------//
// Set * Time                                                                 //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time the file was created.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetCreationTime(const std::string &filename, time_t dateTime) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time, in coordinated universal time (UTC),
///   that the file was created.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetCreationTimeUtc(const std::string &filename, time_t dateTime) noexcept;


///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time the specified file was last accessed.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetLastAccessTime(const std::string &filename, time_t dateTime) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time, in coordinated universal time (UTC),
///   that the specified file was last accessed.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetLastAccessTimeUtc(const std::string &filename, time_t dateTime) noexcept;


///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time that the specified file was last written to.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetLastWriteTime(const std::string &filename, time_t dateTime) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Sets the date and time, in coordinated universal time (UTC),
///   that the specified file was last written to.
/// @param filename
///   The name of target file.
/// @param dateTime
///   The desired date time.
/// @see time_t
void SetLastWriteTimeUtc(const std::string &filename, time_t dateTime) noexcept;


//----------------------------------------------------------------------------//
// Size                                                                       //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @briefs
///   Gets the size of file.
/// @param filename
///   The target filename.
/// @returns
///   The size of file in bytes.
size_t GetSize(const std::string &filename) noexcept;

///-----------------------------------------------------------------------------
/// @briefs
///   Gets the size of file.
/// @param fileStream
///   The target file stream.
/// @returns
///   The size of file in bytes.
size_t GetSize(std::fstream &fileStream) noexcept;


//----------------------------------------------------------------------------//
// Write                                                                      //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Creates a new file, writes the specified byte array to the file,
///   and then closes the file. If the target file already exists,
///   it is overwritten.
/// @param filename
///   The name of the file that will be written.
/// @param bytes
///   The list of bytes that will be written.
/// @see byte_t.
void WriteAllBytes(
    const std::string     &filename,
    const std::vector<u8> &bytes) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Creates a new file, writes a collection of strings to the file,
///   and then closes the file.
/// @param filename
///   The name of the file that will be written.
/// @param lines
///   The list of lines that will be written.
void WriteAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines) noexcept;

///-----------------------------------------------------------------------------
/// @brief
///   Creates a new file, writes the specified string to the file,
///   and then closes the file. If the target file already exists,
///   it is overwritten.
/// @param filename
///   The name of the file that will be written.
/// @param contents
///   The text that will be written.
void WriteAllText(
    const std::string &filename,
    const std::string &contents) noexcept;

} // namespace File
} // namespace IO
} // namespace acow
