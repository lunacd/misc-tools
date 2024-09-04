#pragma once

#include <string>
#include <filesystem>

namespace SysUtil {
std::string getEnv(const std::string &name);

std::filesystem::path getProgramDataDir(std::string_view programName);
}
