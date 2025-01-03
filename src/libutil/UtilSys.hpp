#pragma once

#include <filesystem>
#include <string>

namespace Lunacd::Util::Sys {
std::string getEnv(const std::string &name);

std::filesystem::path getProgramDataDir(std::string_view programName);
} // namespace Util::Sys
