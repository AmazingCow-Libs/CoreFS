//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : Path_Win32.cpp                                              //
//  Project   : Path                                                        //
//  Date      : Aug 01, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017, 2018                                       //
//                                                                            //
//  Description :                                                             //
//    Implementation of the functions for Windows OS.                         //
//    All other functions resides on:                                         //
//      For GNU/Linux - Path_GNU_Linux.cpp                                  //
//      For Windows   - Path_W32.cpp                                        //
//      For OSX       - Path_OSX.cpp                                        //
//      ... and so on...                                                      //
//                                                                            //
//    But please...                                                           //
//    ADD ALL FUNCTIONS SIGNATURES HERE (and it's proper files)               //
//    COMMENTED OUT AND IN THE ORDER THAT IT IS DECLARED IN Path.h.         //
//    This will enable search the files very easily since the structure       //
//    will be the same of every one of them.                                  //
//                                                                            //
//---------------------------------------------------------------------------~//

// Amazing Cow Libs
#include "acow/cpp_goodies.h"

#if (ACOW_OS_IS_WINDOWS)

// Header
#include "os_functions.h"
// std
#include <string>
// Windows
#include <direct.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
#include <Lmcons.h> // For UNLEN (GetUserName)
// AmazingCow Libs
#include "acow/cpp_goodies.h"
// acow_IO
#include "acow/include/Path.h"

// Usings
using namespace acow;
using namespace IO;
using namespace Path;


//----------------------------------------------------------------------------//
// Library Linking                                                            //
//----------------------------------------------------------------------------//
#pragma comment(lib, "Shlwapi.lib")


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
acow_internal_function std::string
ToStdString(PWSTR pwstr) noexcept
{
    std::wstring wstr(pwstr);
    std::string   str(wstr.begin(), wstr.end());

    return str;
}

//------------------------------------------------------------------------------
acow_internal_function std::string
GetFolderPathHelper(GUID folderGUID) noexcept
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
        path = ToStdString(p_buf);

    CoTaskMemFree(p_buf);

    return path;
}


//----------------------------------------------------------------------------//
// Implementation Functions                                                   //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::string
_private::OS_GetTempPath() noexcept
{
    // COWTODO(n2omatt): Bullet proof the function.
    //   See Remarks section about the failures points.
    //   https://msdn.microsoft.com/en-us/library/windows/desktop/aa364992(v=vs.85).aspx
    char buffer[MAX_PATH] = {0};
    GetTempPathA(MAX_PATH, buffer);

    return {buffer};
}

//------------------------------------------------------------------------------
std::string
_private::OS_GetFolderPath(SpecialFolder folder) noexcept
{
    //--------------------------------------------------------------------------
    // References:
    //  SHGetKnownFolderPath
    //    https://msdn.microsoft.com/en-us/library/windows/desktop/bb762188(v=vs.85).aspx
    //  KNOWNFOLDERID
    //    https://msdn.microsoft.com/en-us/library/windows/desktop/dd378457(v=vs.85).aspx
    GUID folder_guid;

    //COWTODO: Recheck the values with the provided by dot net, both on x86 and x64.
    switch(folder) {
        case SpecialFolder::AdminTools             : folder_guid = FOLDERID_AdminTools;            break;
        case SpecialFolder::ApplicationData        : folder_guid = FOLDERID_LocalAppData;          break;
        case SpecialFolder::CDBurning              : folder_guid = FOLDERID_CDBurning;             break;
        case SpecialFolder::CommonAdminTools       : folder_guid = FOLDERID_CommonAdminTools;      break;
        case SpecialFolder::CommonApplicationData  : folder_guid = FOLDERID_ProgramData;           break;
        case SpecialFolder::CommonDesktopDirectory : folder_guid = FOLDERID_PublicDesktop;         break;
        case SpecialFolder::CommonDocuments        : folder_guid = FOLDERID_PublicDocuments;       break;
        case SpecialFolder::CommonMusic            : folder_guid = FOLDERID_PublicMusic;           break;
        case SpecialFolder::CommonOemLinks         : folder_guid = FOLDERID_CommonOEMLinks;        break;
        case SpecialFolder::CommonPictures         : folder_guid = FOLDERID_PublicPictures;        break;
        case SpecialFolder::CommonProgramFiles     : folder_guid = FOLDERID_ProgramFilesCommon;    break;
        case SpecialFolder::CommonProgramFilesX86  : folder_guid = FOLDERID_ProgramFilesCommonX64; break;
        case SpecialFolder::CommonPrograms         : folder_guid = FOLDERID_CommonPrograms;        break;
        case SpecialFolder::CommonStartMenu        : folder_guid = FOLDERID_CommonStartMenu;       break;
        case SpecialFolder::CommonStartup          : folder_guid = FOLDERID_CommonStartup;         break;
        case SpecialFolder::CommonTemplates        : folder_guid = FOLDERID_CommonTemplates;       break;
        case SpecialFolder::CommonVideos           : folder_guid = FOLDERID_PublicVideos;          break;
        case SpecialFolder::Cookies                : folder_guid = FOLDERID_Cookies;               break;
        case SpecialFolder::Desktop                : folder_guid = FOLDERID_Desktop;               break;
        case SpecialFolder::DesktopDirectory       : folder_guid = FOLDERID_Desktop;               break;
        case SpecialFolder::Favorites              : folder_guid = FOLDERID_Favorites;             break;
        case SpecialFolder::Fonts                  : folder_guid = FOLDERID_Fonts;                 break;
        case SpecialFolder::History                : folder_guid = FOLDERID_History;               break;
        case SpecialFolder::InternetCache          : folder_guid = FOLDERID_InternetCache;         break;
        case SpecialFolder::LocalApplicationData   : folder_guid = FOLDERID_LocalAppData;          break;
        case SpecialFolder::LocalizedResources     : folder_guid = FOLDERID_LocalizedResourcesDir; break;
        case SpecialFolder::MyComputer             : folder_guid = FOLDERID_ComputerFolder;        break;
        case SpecialFolder::MyDocuments            : folder_guid = FOLDERID_Documents;             break;
        case SpecialFolder::MyMusic                : folder_guid = FOLDERID_Music;                 break;
        case SpecialFolder::MyPictures             : folder_guid = FOLDERID_Pictures;              break;
        case SpecialFolder::MyVideos               : folder_guid = FOLDERID_Videos;                break;
        case SpecialFolder::NetworkShortcuts       : folder_guid = FOLDERID_NetHood;               break;
        case SpecialFolder::Personal               : folder_guid = FOLDERID_Documents;             break;
        case SpecialFolder::PrinterShortcuts       : folder_guid = FOLDERID_PrintHood;             break;
        case SpecialFolder::ProgramFiles           : folder_guid = FOLDERID_ProgramFilesX64;       break;
        case SpecialFolder::ProgramFilesX86        : folder_guid = FOLDERID_ProgramFilesX86;       break;
        case SpecialFolder::Programs               : folder_guid = FOLDERID_Programs;              break;
        case SpecialFolder::Recent                 : folder_guid = FOLDERID_Recent;                break;
        case SpecialFolder::Resources              : folder_guid = FOLDERID_ResourceDir;           break;
        case SpecialFolder::SendTo                 : folder_guid = FOLDERID_SendTo;                break;
        case SpecialFolder::StartMenu              : folder_guid = FOLDERID_StartMenu;             break;
        case SpecialFolder::Startup                : folder_guid = FOLDERID_Startup;               break;
        case SpecialFolder::System                 : folder_guid = FOLDERID_System;                break;
        case SpecialFolder::SystemX86              : folder_guid = FOLDERID_SystemX86;             break;
        case SpecialFolder::Templates              : folder_guid = FOLDERID_Templates;             break;
        case SpecialFolder::UserProfile            : folder_guid = FOLDERID_Profile;               break;
        case SpecialFolder::Windows                : folder_guid = FOLDERID_Windows;               break;
    }

    return GetFolderPathHelper(folder_guid);
}

//------------------------------------------------------------------------------
std::string
_private::OS_GetAbsPath(const std::string &path) noexcept
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

    auto norm_path = Path::NormPath(std::string(buffer));
    return norm_path;;
}

//------------------------------------------------------------------------------
std::string
_private::OS_GetUserHome(const std::string &username) noexcept
{
    //--------------------------------------------------------------------------
    // COWHACK(n2omatt): Get the CURRENT user name.
    //   This is needed because we don't find a way to get the other users
    //   directory correctly (or easily) on Windows.
    //   So on the same user we do the right thing - That's call the
    //   function that actually gets the home folder.
    //   But if the username is other the the current user, we do a hack
    //   assuming that this correct. First we get the current user home directory,
    //   then we strip the username part and replace with the target user.
    //   Sincerely I don't know if this is correctly, but now we gonna user that
    //   anyways...

    // COWTODO(n2omatt): Bullet proof this...
    //   See the return values section to check what can be wrong.
    //   https://msdn.microsoft.com/en-us/library/windows/desktop/ms724432%28v=vs.85%29.aspx
    DWORD username_buf_size              = UNLEN +1;
    char  current_username_buf[UNLEN +1] = {0};
    GetUserName(current_username_buf, &username_buf_size);

    // Get the CURRENT user home folder.
    auto current_user_home = OS_GetFolderPath(Path::SpecialFolder::UserProfile);
    // Same user - Straight forward...
    if(username.empty() ||  username == current_username_buf) {
        return Path::AbsPath(current_user_home);
    }
    // Other user - we need do some hacks...
    else {
        current_user_home     = Path::AbsPath(current_user_home);
        auto base_home        = Path::Dirname(current_user_home);
        auto target_user_home = Path::Join(base_home, {username});

        if(Path::IsDir(target_user_home)) {
            return target_user_home;
        }

        return "";
    }
}

//------------------------------------------------------------------------------
bool
_private::OS_IsAbs(const std::string &path) noexcept
{
    return !PathIsRelativeA(path.c_str());
}

//------------------------------------------------------------------------------
bool
_private::OS_IsMount(const std::string &path) noexcept
{
    ACOW_NOT_IMPLEMENTED();
    return false;
}

//------------------------------------------------------------------------------
std::string
_private::OS_RelPath(
    const std::string &path,
    const std::string &start) noexcept
{
    ACOW_NOT_IMPLEMENTED();
    return false;
}

#endif // #if (ACOW_OS_IS_WINDOWS)
