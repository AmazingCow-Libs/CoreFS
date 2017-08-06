#include <iostream>
#include "CoreFS.h"

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
