#pragma once

#include <filesystem>

namespace Lunacd::Util::Filesystem {
bool pathIsContainedIn(const std::filesystem::path &path,
                       const std::filesystem::path &root);
} // namespace Lunacd::Util::Filesystem