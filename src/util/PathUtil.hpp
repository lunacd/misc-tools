#pragma once

#include <filesystem>

namespace PathUtil {
std::filesystem::path getProgramDataDir(std::string_view programName);
}
