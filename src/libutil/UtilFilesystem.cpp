#include <UtilFilesystem.hpp>
#include <filesystem>

namespace Lunacd::Util::Filesystem {
bool pathIsContainedIn(const std::filesystem::path &path,
                       const std::filesystem::path &root) {
  const auto relativePath = std::filesystem::relative(path, root);
  for (const auto &segment : relativePath) {
    if (segment == "..") {
      return false;
    }
  }
  return true;
}
} // namespace Lunacd::Util::Filesystem