#pragma once

// std
#include <string>
// acow_IO
#include "acow/include/SpecialFolder.h"

namespace acow { namespace IO { namespace Path { namespace _private {

std::string OS_GetTempPath() noexcept;
std::string OS_GetFolderPath(SpecialFolder folder) noexcept;
std::string OS_GetAbsPath(const std::string &path) noexcept;
std::string OS_GetUserHome(const std::string &username = "") noexcept;

bool OS_IsAbs(const std::string &path) noexcept;
bool OS_IsMount(const std::string &path) noexcept;

std::string OS_RelPath(
    const std::string &path,
    const std::string &start) noexcept;

} // namespace _private
} // namespace Path
} // namespace IO
} // namespace acow