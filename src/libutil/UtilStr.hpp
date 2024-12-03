#pragma once

#include <string>
#include <vector>

namespace Lunacd::Util::Str {
std::string_view trim(std::string_view input);
std::string toLower(std::string_view input);
std::vector<std::string> split(std::string_view input, char delim);
} // namespace Lunacd::Util::Str