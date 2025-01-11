#include <UtilOat.hpp>

#include <fmt/format.h>
#include <optional>

namespace Lunacd::Util::Oat {
std::string
StaticControllerBase::getFile(const std::filesystem::path &filePath) {
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

std::string StaticControllerBase::extToMIME(const std::string &ext) {
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

std::optional<std::string>
getCookie(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request>
              &request,
          std::string_view cookieName) {
  const auto cookieHeader = request->getHeader("Cookie");
  if (!cookieHeader) {
    return std::nullopt;
  }
  const auto rawCookies = Util::Str::split(std::string(cookieHeader), ';');
  for (const auto rawCookie : rawCookies) {
    const auto trimmedRawCookie = Util::Str::trim(rawCookie);
    if (!Util::Str::startsWith(trimmedRawCookie, cookieName)) {
      continue;
    }
    const auto cookieSegments = Util::Str::split(trimmedRawCookie, '=');
    if (cookieSegments.size() != 2) {
      return std::nullopt;
    }
    return std::string(Util::Str::trim(cookieSegments[1]));
  }
  return std::nullopt;
}
} // namespace Lunacd::Util::Oat
