#pragma once

// std
#include <string>
// CoreFS
#include "../../include/CoreFS.h"

std::string os_GetTempPath() noexcept;
std::string os_GetFolderPath(CoreFS::SpecialFolder folder) noexcept;
std::string os_GetAbsPath(const std::string &path) noexcept;
std::string os_GetUserHome(const std::string &username = "") noexcept;

bool os_IsAbs(const std::string &path) noexcept;
bool os_IsMount(const std::string &path) noexcept;

std::string os_RelPath(
	const std::string &path, 
	const std::string &start) noexcept;