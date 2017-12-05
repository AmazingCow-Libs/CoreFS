//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFS_Win32.cpp                                              //
//  Project   : CoreFS                                                        //
//  Date      : Aug 01, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//    Implementation of the functions for Windows OS.                         //
//    All other functions resides on:                                         //
//      For GNU/Linux - CoreFS_GNU_Linux.cpp                                  //
//      For Windows   - CoreFS_W32.cpp                                        //
//      For OSX       - CoreFS_OSX.cpp                                        //
//      ... and so on...                                                      //
//                                                                            //
//    But please...                                                           //
//    ADD ALL FUNCTIONS SIGNATURES HERE (and it's proper files)               //
//    COMMENTED OUT AND IN THE ORDER THAT IT IS DECLARED IN CoreFS.h.         //
//    This will enable search the files very easily since the structure       //
//    will be the same of every one of them.                                  //
//                                                                            //
//---------------------------------------------------------------------------~//

#ifdef _WIN32
// Header
#include "../include/CoreFS.h"
// std
#include <string>
// Windows
#include <direct.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <Windows.h>


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string to_std_string(PWSTR pwstr)
{
    std::wstring wstr(pwstr);
    std::string   str(wstr.begin(), wstr.end());

    return str;
}

//------------------------------------------------------------------------------
std::string get_folder_path_helper(GUID folderGUID)
{
    PWSTR p_buf;

    auto hresult = SHGetKnownFolderPath(
        folderGUID,
        KF_FLAG_DEFAULT,
        nullptr,
        &p_buf
    );

    std::string path;
    if(SUCCEEDED(hresult))
        path = to_std_string(p_buf);

    CoTaskMemFree(p_buf);

    return path;
}

//----------------------------------------------------------------------------//
// C# System.Environment Like API                                             //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string CurrentDirectory()

//------------------------------------------------------------------------------
//  Defined at: CoreFS.cpp
//std::string NewLine()

//------------------------------------------------------------------------------
//  Defined at: CoreFS.cpp
//std::string CoreFS::SystemDirectory();

//------------------------------------------------------------------------------
std::string CoreFS::GetFolderPath(CoreFS::SpecialFolder folder)
{
    //--------------------------------------------------------------------------
    // References:
    //  SHGetKnownFolderPath
    //    https://msdn.microsoft.com/en-us/library/windows/desktop/bb762188(v=vs.85).aspx
    //  KNOWNFOLDERID
    //    https://msdn.microsoft.com/en-us/library/windows/desktop/dd378457(v=vs.85).aspx
    GUID folder_guid;

    //COWTODO: Recheck the values with the provided by dot net, both on x86 and x64.
    switch(folder)
    {
        case CoreFS::SpecialFolder::AdminTools             : folder_guid = FOLDERID_AdminTools;            break;
        case CoreFS::SpecialFolder::ApplicationData        : folder_guid = FOLDERID_LocalAppData;          break;
        case CoreFS::SpecialFolder::CDBurning              : folder_guid = FOLDERID_CDBurning;             break;
        case CoreFS::SpecialFolder::CommonAdminTools       : folder_guid = FOLDERID_CommonAdminTools;      break;
        case CoreFS::SpecialFolder::CommonApplicationData  : folder_guid = FOLDERID_ProgramData;           break;
        case CoreFS::SpecialFolder::CommonDesktopDirectory : folder_guid = FOLDERID_PublicDesktop;         break;
        case CoreFS::SpecialFolder::CommonDocuments        : folder_guid = FOLDERID_PublicDocuments;       break;
        case CoreFS::SpecialFolder::CommonMusic            : folder_guid = FOLDERID_PublicMusic;           break;
        case CoreFS::SpecialFolder::CommonOemLinks         : folder_guid = FOLDERID_CommonOEMLinks;        break;
        case CoreFS::SpecialFolder::CommonPictures         : folder_guid = FOLDERID_PublicPictures;        break;
        case CoreFS::SpecialFolder::CommonProgramFiles     : folder_guid = FOLDERID_ProgramFilesCommon;    break;
        case CoreFS::SpecialFolder::CommonProgramFilesX86  : folder_guid = FOLDERID_ProgramFilesCommonX64; break;
        case CoreFS::SpecialFolder::CommonPrograms         : folder_guid = FOLDERID_CommonPrograms;        break;
        case CoreFS::SpecialFolder::CommonStartMenu        : folder_guid = FOLDERID_CommonStartMenu;       break;
        case CoreFS::SpecialFolder::CommonStartup          : folder_guid = FOLDERID_CommonStartup;         break;
        case CoreFS::SpecialFolder::CommonTemplates        : folder_guid = FOLDERID_CommonTemplates;       break;
        case CoreFS::SpecialFolder::CommonVideos           : folder_guid = FOLDERID_PublicVideos;          break;
        case CoreFS::SpecialFolder::Cookies                : folder_guid = FOLDERID_Cookies;               break;
        case CoreFS::SpecialFolder::Desktop                : folder_guid = FOLDERID_Desktop;               break;
        case CoreFS::SpecialFolder::DesktopDirectory       : folder_guid = FOLDERID_Desktop;               break;
        case CoreFS::SpecialFolder::Favorites              : folder_guid = FOLDERID_Favorites;             break;
        case CoreFS::SpecialFolder::Fonts                  : folder_guid = FOLDERID_Fonts;                 break;
        case CoreFS::SpecialFolder::History                : folder_guid = FOLDERID_History;               break;
        case CoreFS::SpecialFolder::InternetCache          : folder_guid = FOLDERID_InternetCache;         break;
        case CoreFS::SpecialFolder::LocalApplicationData   : folder_guid = FOLDERID_LocalAppData;          break;
        case CoreFS::SpecialFolder::LocalizedResources     : folder_guid = FOLDERID_LocalizedResourcesDir; break;
        case CoreFS::SpecialFolder::MyComputer             : folder_guid = FOLDERID_ComputerFolder;        break;
        case CoreFS::SpecialFolder::MyDocuments            : folder_guid = FOLDERID_Documents;             break;
        case CoreFS::SpecialFolder::MyMusic                : folder_guid = FOLDERID_Music;                 break;
        case CoreFS::SpecialFolder::MyPictures             : folder_guid = FOLDERID_Pictures;              break;
        case CoreFS::SpecialFolder::MyVideos               : folder_guid = FOLDERID_Videos;                break;
        case CoreFS::SpecialFolder::NetworkShortcuts       : folder_guid = FOLDERID_NetHood;               break;
        case CoreFS::SpecialFolder::Personal               : folder_guid = FOLDERID_Documents;             break;
        case CoreFS::SpecialFolder::PrinterShortcuts       : folder_guid = FOLDERID_PrintHood;             break;
        case CoreFS::SpecialFolder::ProgramFiles           : folder_guid = FOLDERID_ProgramFilesX64;       break;
        case CoreFS::SpecialFolder::ProgramFilesX86        : folder_guid = FOLDERID_ProgramFilesX86;       break;
        case CoreFS::SpecialFolder::Programs               : folder_guid = FOLDERID_Programs;              break;
        case CoreFS::SpecialFolder::Recent                 : folder_guid = FOLDERID_Recent;                break;
        case CoreFS::SpecialFolder::Resources              : folder_guid = FOLDERID_ResourceDir;           break;
        case CoreFS::SpecialFolder::SendTo                 : folder_guid = FOLDERID_SendTo;                break;
        case CoreFS::SpecialFolder::StartMenu              : folder_guid = FOLDERID_StartMenu;             break;
        case CoreFS::SpecialFolder::Startup                : folder_guid = FOLDERID_Startup;               break;
        case CoreFS::SpecialFolder::System                 : folder_guid = FOLDERID_System;                break;
        case CoreFS::SpecialFolder::SystemX86              : folder_guid = FOLDERID_SystemX86;             break;
        case CoreFS::SpecialFolder::Templates              : folder_guid = FOLDERID_Templates;             break;
        case CoreFS::SpecialFolder::UserProfile            : folder_guid = FOLDERID_Profile;               break;
        case CoreFS::SpecialFolder::Windows                : folder_guid = FOLDERID_Windows;               break;
    }

    return get_folder_path_helper(folder_guid);
}


//----------------------------------------------------------------------------//
// Python os.path Like API                                                    //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string CoreFS::AbsPath(const std::string &path)
{
    //--------------------------------------------------------------------------
    // Reference:
    //   https://msdn.microsoft.com/en-us/library/windows/desktop/aa364963(v=vs.85).aspx

    constexpr int kBufferSize = 4096;

    DWORD  retval              = 0;
    BOOL   success             = false;
    CHAR   buffer[kBufferSize] = {'\0'};
    CHAR** lppPart             = {NULL};

    retval = GetFullPathNameA(
        path.c_str(),
        kBufferSize,
        buffer,
        lppPart
    );

    auto norm_path = CoreFS::NormPath(std::string(buffer));
    return norm_path;;
}

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string Basename(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string CommonPrefix(const std::initializer_list<std::string> &paths);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string Dirname(const std::string &path)

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//bool Exists(const std::string &path);

//------------------------------------------------------------------------------
std::string CoreFS::ExpandUser(const std::string &path)
{
    //COWTODO(n2omatt): Implement...
    return "";
}

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//time_t GetATime(const std::string &filename);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//time_t GetCTime(const std::string &filename);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//time_t GetMTime(const std::string &filename);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//long int GetSize(const std::string &filename);

//------------------------------------------------------------------------------
bool CoreFS::IsAbs(const std::string &path)
{
    return !PathIsRelativeA(path.c_str());
}

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//bool IsDir(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//bool IsFile(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//bool IsLink(const std::string &path);

//------------------------------------------------------------------------------
bool CoreFS::IsMount(const std::string &path)
{
    //COWTODO(n2omatt): Implement...
    return false;
}

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string Join(const std::vector<std::string> &paths);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string Join(
//    const std::string &path,
//    const std::vector<std::string> &paths);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//bool LExists(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string NormCase(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::string NormPath(const std::string &path);

//------------------------------------------------------------------------------
std::string CoreFS::RelPath(
    const std::string &path,
    const std::string &start /* = "." */)
{
    //COWTODO(n2omatt): Implement...
    return "";
}

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::pair<std::string, std::string> Split(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::pair<std::string, std::string> Split(const std::string &path);

//------------------------------------------------------------------------------
//  Defined in CoreFS.cpp
//std::pair<std::string, std::string> SplitExt(const std::string &path);

#endif //_WIN32
