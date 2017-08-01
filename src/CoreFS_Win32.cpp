//COWTODO: Add license headers...
//
//Notice:
//  Implementation of the functions for Windows OS.
//  All other functions resides on:
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

#ifdef _WIN32
//Header
#include "../include/CoreFS.h"
//std
#include <string>
//Windows
#include <Windows.h>
#include <direct.h>
#include <Shlobj.h>

#define SAFE_FREE(_ptr_) do { \
    if(_ptr_ != nullptr) {    \
        free(_ptr_);          \
        _ptr_ = nullptr;      \
    }                         \
} while(0);


////////////////////////////////////////////////////////////////////////////////
// Helper Functions                                                           //
////////////////////////////////////////////////////////////////////////////////
std::string to_std_string(PWSTR pwstr)
{
    std::wstring wstr(pwstr);
    std::string   str(wstr.begin(), wstr.end());

    return str;
}

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

////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
std::string CoreFS::CurrentDirectory()
{
    //COWTODO: Add an assertion on the pbuf value.
    auto p_buf = _getcwd(nullptr, 0);
    auto cwd   = std::string(p_buf);

    SAFE_FREE(p_buf);

    return cwd;
}

//Defined at: CoreFS.cpp
//std::string NewLine()

std::string CoreFS::SystemDirectory()
{
   return get_folder_path_helper(FOLDERID_System);
}

std::string CoreFS::GetFolderPath(CoreFS::SpecialFolder folder)
{
    //References:
    //SHGetKnownFolderPath
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/bb762188(v=vs.85).aspx
    //KNOWNFOLDERID
    //  https://msdn.microsoft.com/en-us/library/windows/desktop/dd378457(v=vs.85).aspx
    GUID folder_guid;

    //COWTODO: Finish Implementation.
    switch(folder)
    {
        case CoreFS::SpecialFolder::AdminTools             : folder_guid = FOLDERID_AdminTools;       break;
        case CoreFS::SpecialFolder::ApplicationData        : /*folder_guid = ;*/ break;
        case CoreFS::SpecialFolder::CDBurning              : folder_guid = FOLDERID_CDBurning;        break;
        case CoreFS::SpecialFolder::CommonAdminTools       : folder_guid = FOLDERID_CommonAdminTools; break;
        case CoreFS::SpecialFolder::CommonApplicationData  : /*folder_guid = ;*/ break;
        case CoreFS::SpecialFolder::CommonDesktopDirectory : folder_guid = FOLDERID_PublicDesktop;   break;
        case CoreFS::SpecialFolder::CommonDocuments        : folder_guid = FOLDERID_PublicDocuments; break;
        case CoreFS::SpecialFolder::CommonMusic            : folder_guid = FOLDERID_PublicMusic;     break;
        case CoreFS::SpecialFolder::CommonOemLinks         : folder_guid = FOLDERID_CommonOEMLinks;  break;
        case CoreFS::SpecialFolder::CommonPictures         : folder_guid = FOLDERID_PublicPictures;  break;
        case CoreFS::SpecialFolder::CommonProgramFiles     : /*folder_guid = ; */break;
        case CoreFS::SpecialFolder::CommonProgramFilesX86  : /*folder_guid = ; */break;
        case CoreFS::SpecialFolder::CommonPrograms         : folder_guid = FOLDERID_CommonPrograms; break;
        case CoreFS::SpecialFolder::CommonStartMenu        : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::CommonStartup          : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::CommonTemplates        : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::CommonVideos           : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::Cookies                : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::Desktop                : folder_guid = FOLDERID_Desktop; break;
        case CoreFS::SpecialFolder::DesktopDirectory       : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::Favorites              : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::Fonts                  : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::History                : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::InternetCache          : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::LocalApplicationData   : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::LocalizedResources     : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::MyComputer             : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::MyDocuments            : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::MyMusic                : folder_guid = FOLDERID_Music; break;
        case CoreFS::SpecialFolder::MyPictures             : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::MyVideos               : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::NetworkShortcuts       : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::Personal               : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::PrinterShortcuts       : /*folder_guid = */; break;
        case CoreFS::SpecialFolder::ProgramFiles           : folder_guid = FOLDERID_ProgramFilesX64; break;
        case CoreFS::SpecialFolder::ProgramFilesX86        : folder_guid = FOLDERID_ProgramFilesX86; break;
        case CoreFS::SpecialFolder::Programs               : folder_guid = FOLDERID_Programs;        break;
        case CoreFS::SpecialFolder::Recent                 : folder_guid = FOLDERID_Recent;          break;
        case CoreFS::SpecialFolder::Resources              : folder_guid = FOLDERID_ResourceDir;     break;
        case CoreFS::SpecialFolder::SendTo                 : folder_guid = FOLDERID_SendTo;          break;
        case CoreFS::SpecialFolder::StartMenu              : folder_guid = FOLDERID_StartMenu;       break;
        case CoreFS::SpecialFolder::Startup                : folder_guid = FOLDERID_Startup;         break;
        case CoreFS::SpecialFolder::System                 : folder_guid = FOLDERID_System;          break;
        case CoreFS::SpecialFolder::SystemX86              : folder_guid = FOLDERID_SystemX86;       break;
        case CoreFS::SpecialFolder::Templates              : folder_guid = FOLDERID_Templates;       break;
        case CoreFS::SpecialFolder::UserProfile            : /*folder_guid = ;*/ break;
        case CoreFS::SpecialFolder::Windows                : folder_guid = FOLDERID_Windows;         break;
    }

    return get_folder_path_helper(folder_guid);
}

std::vector<std::string> CoreFS::GetLogicalDrives()
{
    //COWTODO: not implemented.
    return std::vector<std::string>();
}
#endif //_WIN32
