#include <UtilOat.hpp>

#include <fmt/format.h>

namespace Lunacd::Util::Oat {
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
    if (!trimmedRawCookie.starts_with(cookieName)) {
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
