module;

#include <mutex>
#include <filesystem>

export module lunacd.util:sysUtil;

namespace SysUtil {
export std::string getEnv(const std::string &name) {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock{mutex};
  const char *value = std::getenv(name.c_str());
  return value != nullptr ? value : std::string{};
}

std::filesystem::path getUserDataDir() {
  if (const auto xdgDataHome = getEnv("XDG_DATA_HOME"); !xdgDataHome.empty()) {
    return xdgDataHome;
  }

#ifdef _WIN32
  static std::string homeEnvVar = "USERPROFILE";
#else
  static std::string homeEnvVar = "HOME";
#endif

  return std::filesystem::path(getEnv(homeEnvVar)) / ".local" / "share";
}

export std::filesystem::path getProgramDataDir(std::string_view programName) {
  return getUserDataDir() / programName;
}
} // namespace SysUtil
