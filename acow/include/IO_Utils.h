#pragma once

#include "acow/cpp_goodies.h"

//----------------------------------------------------------------------------//
// Get rid of the Windows macros.                                             //
//----------------------------------------------------------------------------//
#if (ACOW_OS_IS_WINDOWS)
    #undef GetTempFileName
    #undef GetTempPath
    #undef GetCurrentDirectory
    #undef SetCurrentDirectory
#endif