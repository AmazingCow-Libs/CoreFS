//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : File.cpp                                                  //
//  Project   : File                                                      //
//  Date      : Aug 26, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

// Header
#include "acow/include/File.h"
// std
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iterator>
#include <sstream>
// acow_IO
#include "acow/include/Path.h"

// Usings
using namespace acow;
using namespace IO;


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
// COWNOTE(n2omatt): Convert the time_t value to struct tm
//   This is needed because the pointers that localtime(3) or gmtime(3)
//   return might be reused in subsequent calls to the functions.
acow_internal_function tm_t
time2tm(time_t time, bool local) noexcept
{
    auto p_tm = (local) ? localtime(&time) : gmtime(&time);
    tm_t ret_tm = {0};
    memcpy(&ret_tm, p_tm, sizeof(ret_tm));

    return ret_tm;
}

acow_internal_function std::fstream::openmode
FileMode2OpenMode(const std::string &filemode) noexcept
{
    //--------------------------------------------------------------------------
    // Text mode.
    if(filemode == File::FileMode::Text::kRead)
        return std::ios::in;

    if(filemode == File::FileMode::Text::kWrite)
        return std::ios::out;

    if(filemode == File::FileMode::Text::kReadWrite_Open)
        return std::ios::in | std::ios::out;
    if(filemode == File::FileMode::Text::kReadWrite_Truncate)
        return std::ios::in | std::ios::out | std::ios::trunc;

    if(filemode == File::FileMode::Text::kAppend)
        return std::ios::in | std::ios::app;
    if(filemode == File::FileMode::Text::kAppend_Truncate)
        return std::ios::in | std::ios::app | std::ios::trunc;

    //--------------------------------------------------------------------------
    // Binary mode.
    if(filemode == File::FileMode::Binary::kRead)
        return std::ios::in | std::ios::binary;

    if(filemode == File::FileMode::Binary::kWrite)
        return std::ios::out | std::ios::binary;

    if(filemode == File::FileMode::Binary::kReadWrite_Open)
        return std::ios::in | std::ios::out | std::ios::binary;
    if(filemode == File::FileMode::Binary::kReadWrite_Truncate)
        return std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary;

    if(filemode == File::FileMode::Binary::kAppend)
        return std::ios::in | std::ios::app | std::ios::binary;
    if(filemode == File::FileMode::Binary::kAppend_Truncate)
        return std::ios::in | std::ios::app | std::ios::trunc | std::ios::binary;

    ACOW_ASSERT(false, "Invalid filemode: (%s)", filemode.c_str());
}


//----------------------------------------------------------------------------//
// Append                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void
File::AppendAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines) noexcept
{
    std::stringstream ss;
    std::string new_line = Path::NewLine();

    //--------------------------------------------------------------------------
    // Join the lines into a single string.
    std::copy(
        lines.begin(),
        lines.end  (),
        std::ostream_iterator<std::string>(ss, new_line.c_str())
    );

    File::AppendAllText(filename, ss.str());
}

//------------------------------------------------------------------------------
void
File::AppendAllText(
    const std::string &filename,
    const std::string &contents) noexcept
{
    // COWTODO(n2omatt): How to handle errors.
    auto p_file_stream = File::Open(filename, FileMode::Text::kAppend);
    (*p_file_stream) << contents;
}


//----------------------------------------------------------------------------//
// Copy                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void
File::Copy(
    const std::string &src,
    const std::string &dst,
    bool               overwrite /* = false */) noexcept
{
    auto src_exists = Path::Exists(src);
    ACOW_ASSERT(src_exists, "Source file doesn't exists - src: (%s)", src.c_str());

    auto dst_exists = Path::Exists(src);
    ACOW_ASSERT(src_exists, "Destination file doesn't exists - src: (%s)", src.c_str());

    // COWTODO(n2omatt): How to handle errors.
    if(!src_exists || !dst_exists) { return; }

    File::WriteAllBytes(
        dst,
        File::ReadAllBytes(src)
    );
}


//----------------------------------------------------------------------------//
// Create                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::Create(const std::string &filename) noexcept
{
    return File::Open(filename, FileMode::Binary::kReadWrite_Truncate);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::CreateText(const std::string &filename) noexcept
{
    return File::Open(filename, FileMode::Text::kReadWrite_Truncate);
}


//----------------------------------------------------------------------------//
// Delete                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void
File::Delete(const std::string &filename) noexcept
{
    auto file_exists = Path::Exists(filename);
    ACOW_ASSERT(file_exists, "File doesn't exists - filename: (%s)", filename.c_str());

    if(!file_exists) { return; }

    auto result = remove(filename.c_str());
    ACOW_ASSERT(result == 0, "Failed to Delete file - filename: (%s)", filename.c_str());
}


//----------------------------------------------------------------------------//
// Exists                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
bool
File::Exists(const std::string &filename) noexcept
{
    return Path::IsFile(filename);
}


//COWTODO: Gonna implement that???
//GetAttributes(const std::string &filename) ???


//----------------------------------------------------------------------------//
// Get * Time                                                                 //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
tm_t
File::GetCreationTime(const std::string &filename) noexcept
{
    return time2tm(Path::GetCTime(filename), true);
}

//------------------------------------------------------------------------------
tm_t
File::GetCreationTimeUtc(const std::string &filename) noexcept
{
    return time2tm(Path::GetCTime(filename), false);
}

//------------------------------------------------------------------------------
tm_t
File::GetLastAccessTime(const std::string &filename) noexcept
{
    return time2tm(Path::GetATime(filename), true);
}

//------------------------------------------------------------------------------
tm_t
File::GetLastAccessTimeUtc(const std::string &filename) noexcept
{
    return time2tm(Path::GetATime(filename), false);
}

//------------------------------------------------------------------------------
tm_t
File::GetLastWriteTime(const std::string &filename) noexcept
{
    return time2tm(Path::GetMTime(filename), true);
}

//------------------------------------------------------------------------------
tm_t
File::GetLastWriteTimeUtc(const std::string &filename) noexcept
{
    return time2tm(Path::GetMTime(filename), false);
}


//----------------------------------------------------------------------------//
// Move                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void
File::Move(
    const std::string &src,
    const std::string &dst,
    bool               overwrite /* = false */) noexcept
{
    auto src_exists = Path::Exists(src);
    ACOW_ASSERT(src_exists, "Source file doesn't exists - src: (%s)", src.c_str());

    auto dst_exists = Path::Exists(src);
    ACOW_ASSERT(src_exists, "Destination file doesn't exists - src: (%s)", src.c_str());

    // COWTODO(n2omatt): How to handle errors.
    if(!src_exists || !dst_exists) { return; }

    auto result = rename(src.c_str(), dst.c_str());
    ACOW_ASSERT(
        result == 0,
        "Failed to Rename file - src: (%s) -> dst: (%s)",
        src.c_str(),
        dst.c_str()
    );
}


//----------------------------------------------------------------------------//
// Open                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::Open(
    const std::string &filename,
    const std::string &filemode) noexcept
{
    auto openmode = FileMode2OpenMode(filemode);
    auto p_stream = std::unique_ptr<std::fstream>(new std::fstream());

    //COWTODO(n2omatt): Check if could alloc the p_stream.
    p_stream->open(filename.c_str(), openmode);
    ACOW_ASSERT(
        p_stream->is_open(),
        "Failed to open stream - filename: (%s) - filemode (%s)",
        filename.c_str(),
        filemode.c_str()
    );

    return p_stream;
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::OpenRead(const std::string &filename) noexcept
{
    return Open(filename, FileMode::Binary::kRead);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::OpenText(const std::string &filename) noexcept
{
    return Open(filename, FileMode::Text::kRead);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>
File::OpenWrite(const std::string &filename) noexcept
{
    return Open(filename, FileMode::Binary::kWrite);
}


//----------------------------------------------------------------------------//
// Read                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::vector<u8>
File::ReadAllBytes(const std::string &filename) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors??
    std::vector<u8> ret_val;

    auto file_exists = File::Exists(filename);

    ACOW_ASSERT(file_exists, "File doesn't exists - filename: (%s)", filename.c_str());
    if(!file_exists) { return ret_val; }

    // Open the file and calculate it's size.
    auto p_file_stream = File::OpenRead(filename);
    auto size          = File::GetSize (*p_file_stream);

    // Play nice with memory.
    ret_val.resize(size);
    p_file_stream->read((char *)(&ret_val[0]), size);

    return ret_val;
}

//------------------------------------------------------------------------------
std::vector<std::string>
File::ReadAllLines(const std::string &filename) noexcept
{
    // COWTODO(n2omatt): How we gonna handle errors??
    std::vector<std::string> ret_val;

    auto file_exists = File::Exists(filename);

    ACOW_ASSERT(file_exists, "File doesn't exists - filename: (%s)", filename.c_str());
    if(!file_exists) { return ret_val; }

    //Open the file and calculate it's size.
    auto p_file_stream = File::OpenText(filename);
    while(!p_file_stream->eof()) {
        std::string line;
        std::getline(*p_file_stream, line);
        ret_val.push_back(line);
    }

    return ret_val;
}

//------------------------------------------------------------------------------
std::string
File::ReadAllText(const std::string &filename) noexcept
{
    // COWTODO(n2omatt): How we gonna handle errors??
    std::string ret_val;

    auto file_exists = File::Exists(filename);

    ACOW_ASSERT(file_exists, "File doesn't exists - filename: (%s)", filename.c_str());
    if(!file_exists) { return ret_val; }

    auto bytes = File::ReadAllBytes(filename);
    std::copy(
        bytes.begin(),
        bytes.end  (),
        std::back_inserter(ret_val)
    );

    return ret_val;
}



//COWTODO(n2omatt): Check how to implement that....
//Replace(const std::string &filename, const std::string &filename, const std::string &filename, Boolean) ???


//----------------------------------------------------------------------------//
// Set * Time                                                                 //
//----------------------------------------------------------------------------//
void
File::SetCreationTime(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    //http://rosettacode.org/wiki/File_modification_time#C.2B.2B
    //utimensat
    ACOW_NOT_IMPLEMENTED();
}

void
File::SetCreationTimeUtc(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    ACOW_NOT_IMPLEMENTED();
}


void
File::SetLastAccessTime(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    ACOW_NOT_IMPLEMENTED();
}

void
File::SetLastAccessTimeUtc(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    ACOW_NOT_IMPLEMENTED();
}


void
File::SetLastWriteTime(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    ACOW_NOT_IMPLEMENTED();
}

void
File::SetLastWriteTimeUtc(const std::string &filename, time_t dateTime) noexcept
{
    //COWTODO(n2omatt): Implement...
    ACOW_NOT_IMPLEMENTED();
}


//----------------------------------------------------------------------------//
// Size                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
size_t
File::GetSize(const std::string &filename) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors???
    auto file_exists = File::Exists(filename);

    ACOW_ASSERT(file_exists, "File doesn't exists - filename: (%s)", filename.c_str());
    if(!file_exists) { return 0; }

    auto p_stream = File::OpenRead(filename);
    return File::GetSize(*p_stream);
}

//------------------------------------------------------------------------------
size_t
File::GetSize(std::fstream &fileStream) noexcept
{
    // COWTODO(n2omatt): How to handle errors.
    auto curr = fileStream.tellg();

    fileStream.seekg(0, std::ios::beg);
    auto beg = fileStream.tellg();

    fileStream.seekg(0, std::ios::end);
    auto end = fileStream.tellg();

    fileStream.seekg(curr, std::ios::beg);

    auto size = static_cast<size_t>(end - beg);
    return size;
}


//----------------------------------------------------------------------------//
// Write                                                                      //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void
File::WriteAllBytes(
    const std::string     &filename,
    const std::vector<u8> &bytes) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = File::Open(
        filename,
        FileMode::Binary::kReadWrite_Truncate
    );

    std::copy(
        bytes.begin(),
        bytes.end  (),
        std::ostream_iterator<u8>(*p_file_stream)
    );
}

//------------------------------------------------------------------------------
void
File::WriteAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = File::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        lines.begin(),
        lines.end  (),
        std::ostream_iterator<std::string>(
            *p_file_stream,
            Path::NewLine().c_str()
        )
    );
}

//------------------------------------------------------------------------------
void
File::WriteAllText(
    const std::string &filename,
    const std::string &contents) noexcept
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = File::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        contents.begin(),
        contents.end  (),
        std::ostream_iterator<char>(*p_file_stream)
    );
}
