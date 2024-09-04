#include <SysUtil.hpp>

#include <mutex>

namespace SysUtil {
std::string getEnv(const std::string &name) {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock{mutex};
  const char *value = std::getenv(name.c_str());
  return value != nullptr ? value : std::string{};
}
} // namespace SysUtil
