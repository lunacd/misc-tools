#include <PathUtil.hpp>

#include <SysUtil.hpp>

namespace PathUtil {
namespace {
std::filesystem::path getUserDataDir() {
  if (const auto xdgDataHome = SysUtil::getEnv("XDG_DATA_HOME");
      !xdgDataHome.empty()) {
    return xdgDataHome;
  }
  return std::filesystem::path(SysUtil::getEnv("HOME")) / ".local" / "share";
}
} // namespace

std::filesystem::path getProgramDataDir(std::string_view programName) {
  return getUserDataDir() / programName;
}
} // namespace PathUtil
