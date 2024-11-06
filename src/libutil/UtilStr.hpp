#pragma once

#include <string>

namespace Util::Str {
std::string_view trim(std::string_view input);
std::string toLower(std::string_view input);
} // namespace Util::Str