//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : main.cpp                                                      //
//  Project   : CoreFS                                                        //
//  Date      : Aug 05, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#include <iostream>
#include "CoreFS/CoreFS.h"

int main()
{
    for(int i = 0; i < static_cast<int>(CoreFS::SpecialFolder::Windows); ++i)
    {
        if(i == 44)
        {
            int c = 1;
        }
        auto special_folder = static_cast<CoreFS::SpecialFolder>(i);
        auto path = CoreFS::GetFolderPath(special_folder);

        std::cout << "(" << i << "): " << std::endl;
        std::cout << "\t" << path << std::endl;

    }

//    std::cout << CoreFS::GetFolderPath(CoreFS::SpecialFolder::MyMusic) << std::endl;

    return 0;
}
