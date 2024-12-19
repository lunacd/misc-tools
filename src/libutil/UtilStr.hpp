#pragma once

#include <string>
#include <vector>

namespace Lunacd::Util::Str {
std::string_view trim(std::string_view input);
std::string toLower(std::string_view input);
std::vector<std::string> split(std::string_view input, char delim);
bool startsWith(std::string_view str, std::string_view prefix);
} // namespace Lunacd::Util::Str