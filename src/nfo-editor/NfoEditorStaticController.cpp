#include <NfoEditorStaticController.hpp>

#include <filesystem>
#include <mutex>
#include <oatpp/core/Types.hpp>
#include <stdexcept>

#include <fmt/format.h>

namespace Lunacd::NfoEditor {
std::string StaticController::getFile(const std::filesystem::path &filePath) {
  if (!std::filesystem::is_regular_file(filePath)) {
    throw std::runtime_error(
        fmt::format("{} does not exist", filePath.string()));
  }
  const auto canonicalPath = std::filesystem::canonical(filePath);

  std::lock_guard<std::mutex> lock{m_cacheLock};
  const auto it = m_cache.find(canonicalPath);
  std::string buffer;
  if (it == m_cache.end()) {
    buffer = oatpp::String::loadFromFile(canonicalPath.c_str());
    if (!buffer.empty()) {
      m_cache[canonicalPath] = buffer;
    }
  } else {
    buffer = it->second;
  }
  return buffer;
}

std::string StaticController::extToMIME(const std::string &ext) {
  if (ext == ".html") {
    return "text/html";
  }
  if (ext == ".css") {
    return "text/css";
  }
  if (ext == ".js") {
    return "text/javascript";
  }
  if (ext == ".svg") {
    return "image/svg+xml";
  }
  throw std::runtime_error(fmt::format("Unknown file type: {}", ext));
};
} // namespace Lunacd::NfoEditor