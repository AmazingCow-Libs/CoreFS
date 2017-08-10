//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        CoreFS_GNU_Linux.cpp                      //
//            █ █        █ █        CoreFS                                    //
//             ████████████                                                   //
//           █              █       Copyright (c) 2017                        //
//          █     █    █     █      AmazingCow - www.AmazingCow.com           //
//          █     █    █     █                                                //
//           █              █       N2OMatt - n2omatt@amazingcow.com          //
//             ████████████         www.amazingcow.com/n2omatt                //
//                                                                            //
//                  This software is licensed as GPLv3                        //
//                 CHECK THE COPYING FILE TO MORE DETAILS                     //
//                                                                            //
//    Permission is granted to anyone to use this software for any purpose,   //
//   including commercial applications, and to alter it and redistribute it   //
//               freely, subject to the following restrictions:               //
//                                                                            //
//     0. You **CANNOT** change the type of the license.                      //
//     1. The origin of this software must not be misrepresented;             //
//        you must not claim that you wrote the original software.            //
//     2. If you use this software in a product, an acknowledgment in the     //
//        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
//        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
//        If you will not acknowledge, just send us a email. We'll be         //
//        *VERY* happy to see our work being used by other people. :)         //
//        The email is: acknowledgment_opensource@AmazingCow.com              //
//     3. Altered source versions must be plainly marked as such,             //
//        and must not be misrepresented as being the original software.      //
//     4. This notice may not be removed or altered from any source           //
//        distribution.                                                       //
//     5. Most important, you must have fun. ;)                               //
//                                                                            //
//      Visit opensource.amazingcow.com for more open-source projects.        //
//                                                                            //
//                                  Enjoy :)                                  //
//----------------------------------------------------------------------------//

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

#ifdef __linux__
//Header
#include "../include/CoreFS.h"
//C
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
//std
#include <cstdlib>
#include <string>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// Helper Functions                                                           //
////////////////////////////////////////////////////////////////////////////////
std::string _read_env(const std::string &env)
{
    auto p_value = getenv(env.c_str());
    if(!p_value)
        return "";
    return p_value;
}

std::string _read_xdg_user_dir(
        const std::string &config_dir,
        const std::string &home_dir,
        const std::string &key,
        const std::string &fallback)
{
    //COWNOTE(n2omatt): Tried to follow the implementation at:
    //  ./mono/mcs/class/corlib/System/Environment.cs
    //Of the function ReadXdgUserDir

    //Check if the XDG key is on environment vars first.
    //  If so, just return it's value.
    auto env_path = _read_env(key);
    if(!env_path.empty())
        return env_path;

    //According with the XDG specs there's a file located
    //on XDG_CONFIG_HOME/user-dirs.dirs that has the definitions
    //for some "well known" directories.
    //
    //The specification is at:
    //  https://www.freedesktop.org/wiki/Software/xdg-user-dirs/
    //
    //So now, we gonna try to parse the file to check if the
    //requested key is defined there.
    auto full_config_path = CoreFS::Join(config_dir, {"user-dirs.dirs"});
    if(!CoreFS::Exists(full_config_path)) //There's no such file.
        return CoreFS::Join(home_dir, {fallback});

    std::ifstream in_file(full_config_path, std::ios::in);
    while(!in_file.eof())
    {
        std::string line;
        std::getline(in_file, line);

        if(line.empty())
            continue;

        //The keys/values on the user-dirs.dirs file is defined to be:
        //  XDG_XXX_DIR=$HOME/YYYY
        //So we gonna try to get the index the current requested key
        //plus the '=' char.
        auto key_index = line.find(key + "=");

        //Not found.
        if(key_index == std::string::npos)
            continue;

        //Now we have the start index of XDG_XXX_DIR key
        //  It might be have whitespaces on the left yet,
        //  or might start with the '#' char meaning thet
        //  it's commented out, so we must ignore it.
        //So check if in this range of [start_of_line, key_index)
        //we have the '#' char.
        auto is_commented = false;
        for(int i = 0; i < key_index; ++i)
        {
            if(line[i] == '#')
            {
                is_commented = true;
                break;
            }
        }

        if(is_commented)
            continue;


        //Now we're sure that even padded with whitespaces
        //the key value is almost valid, we just need to
        //check if we have the $HOME string.
        //This means that path is relative, otherwise it's
        //assumed to be on absolute form.
        auto value_component = line.substr(
             key_index + key.size() + 1 //1 is the '=' separator.
        );

        //Notice that the path is enclosed in quotes, so
        //we need take it in account.
        auto is_relative = value_component.find("$HOME") == 1;
        auto start_index = is_relative ? 6 /* len("$HOME) == 6 */ : 1;
        auto end_index   = value_component.size() - 1;

        auto path = value_component.substr(start_index, end_index - start_index);

        if(is_relative)
            return CoreFS::Join(home_dir, {path});

        return path;
    }

    return CoreFS::Join(home_dir, {fallback});
}


////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
//  Defined at: CoreFS.cpp
//std::string NewLine()

//  Defined at: CoreFS.cpp
//std::string SystemDirectory()

std::string CoreFS::GetFolderPath(CoreFS::SpecialFolder folder)
{
    //COWNOTE(n2omatt):
    //  I'm trying to follow the maximum possible the Mono implementation.
    //  I'm referring to file:
    //    ./mono/mcs/class/corlib/System/Environment.cs

    std::string home_dir = CoreFS::ExpandUser("~");
    std::string folder_path;

    auto data_dir = _read_env("XDG_DATA_HOME");
    if(data_dir.empty())
        data_dir = CoreFS::Join(home_dir, {".local", "share"});

    auto config_dir = _read_env("XDG_CONFIG_HOME");
    if(config_dir.empty())
        config_dir = CoreFS::Join(home_dir, {".config"});

    switch(folder)
    {
        case SpecialFolder::ApplicationData       : folder_path = config_dir;   break;
        case SpecialFolder::LocalApplicationData  : folder_path = data_dir;     break;
        case SpecialFolder::CommonApplicationData : folder_path = "/usr/share"; break;

        // Personal / User Profile
        case SpecialFolder::Personal    :
        case SpecialFolder::UserProfile : {
            folder_path = home_dir;
        } break;

        // Desktop
        case SpecialFolder::Desktop          :
        case SpecialFolder::DesktopDirectory : {
            folder_path = _read_xdg_user_dir(
               config_dir, home_dir,
               "XDG_DESKTOP_DIR", "Desktop"
            );
        } break;

        // Music
        case SpecialFolder::MyMusic : {
            folder_path = _read_xdg_user_dir(
                config_dir, home_dir,
                "XDG_MUSIC_DIR", "Music"
            );
        } break;

        // Pictures
        case SpecialFolder::MyPictures : {
            folder_path = _read_xdg_user_dir(
                config_dir, home_dir,
                "XDG_PICTURES_DIR", "Pictures"
            );
        } break;

        // Videos
        case SpecialFolder::MyVideos: {
            folder_path = _read_xdg_user_dir(
                config_dir, home_dir,
                "XDG_VIDEOS_DIR", "Videos"
            );
        } break;

        // Templates
        case SpecialFolder::Templates: {
            folder_path = _read_xdg_user_dir(
                config_dir, home_dir,
                "XDG_TEMPLATES_DIR", "Templates"
            );
        } break;

        case SpecialFolder::CommonTemplates: {
            folder_path = "/usr/share/templates";
        } break;

        //Fonts
        case SpecialFolder::Fonts : {
            folder_path = CoreFS::Join(home_dir, {".fonts"});
        } break;

        //Not defined on GNU/Linux.
        case SpecialFolder::AdminTools             :
        case SpecialFolder::CDBurning              :
        case SpecialFolder::CommonAdminTools       :
        case SpecialFolder::CommonDesktopDirectory :
        case SpecialFolder::CommonDocuments        :
        case SpecialFolder::CommonMusic            :
        case SpecialFolder::CommonOemLinks         :
        case SpecialFolder::CommonPictures         :
        case SpecialFolder::CommonProgramFiles     :
        case SpecialFolder::CommonProgramFilesX86  :
        case SpecialFolder::CommonPrograms         :
        case SpecialFolder::CommonStartMenu        :
        case SpecialFolder::CommonStartup          :
        case SpecialFolder::CommonVideos           :
        case SpecialFolder::Cookies                :
        case SpecialFolder::History                :
        case SpecialFolder::LocalizedResources     :
        case SpecialFolder::MyComputer             :
        case SpecialFolder::NetworkShortcuts       :
        case SpecialFolder::PrinterShortcuts       :
        case SpecialFolder::ProgramFilesX86        :
        case SpecialFolder::Programs               :
        case SpecialFolder::Recent                 :
        case SpecialFolder::Resources              :
        case SpecialFolder::SendTo                 :
        case SpecialFolder::StartMenu              :
        case SpecialFolder::Startup                :
        case SpecialFolder::System                 :
        case SpecialFolder::SystemX86              :
        case SpecialFolder::Windows                :
        case SpecialFolder::Favorites              :
        case SpecialFolder::ProgramFiles           :
        case SpecialFolder::InternetCache          :
            break;
    }

    return folder_path;
}

std::vector<std::string> CoreFS::GetLogicalDrives()
{
    //COWTODO: not implemented.
    return std::vector<std::string>();
}

////////////////////////////////////////////////////////////////////////////////
// C# System.Environment Like API                                             //
////////////////////////////////////////////////////////////////////////////////
std::string CoreFS::ExpandUser(const std::string &path)
{
    //COWNOTE(n2omatt): Following the python's os.path.expand
    //  user located at: /usr/lib/python2.7/posixpath.py

    if(path.size() == 0 || path[0] != '~')
        return path;

    std::string home_path;

    auto first_slash_index = path.find(CoreFS::GetPathSeparator());

    //There's no slashes on path.
    if(first_slash_index == std::string::npos)
        first_slash_index = path.size();

    if(first_slash_index == 1) //Path is in format of: ~/something/very/nice
    {
        //Try first get the Environment variable.
        auto p_result = getenv("HOME");
        if(!p_result)
        {
            //If it fails try to get the userhome.
            auto uid      = getuid  ();
            auto p_passwd = getpwuid(uid);

            //Failed to retrieve the home path both
            //from environment var and passwd database.
            //  Just return the original path.
            if(!p_passwd)
                return path;

            p_result = p_passwd->pw_dir;
        }

        home_path = p_result;
    }
    else //Path is in format of: ~some_user/something/very/nice
    {
        auto username = path.substr(1, first_slash_index);
        auto p_passwd = getpwnam(username.c_str());

        //User not found...
        if(!p_passwd)
            return path;

        home_path = p_passwd->pw_dir;
    }

    return CoreFS::Join(
        home_path,
        {path.substr(first_slash_index, path.size() - first_slash_index)}
    );
}

#endif //__linux__
