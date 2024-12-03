#include <UtilStr.hpp>

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>

namespace Lunacd::Util::Str {
std::string_view trim(std::string_view input) {
  const auto isNotSpace = [](const unsigned char ch) {
    return std::isspace(ch) == 0;
  };
  const auto firstNonSpacesChar = std::ranges::find_if(input, isNotSpace);
  const size_t prefixLength = std::distance(input.begin(), firstNonSpacesChar);
  input.remove_prefix(prefixLength);

  const auto lastNonSpaceChar =
      std::find_if(input.rbegin(), input.rend(), isNotSpace);
  const size_t suffixLength = std::distance(input.rbegin(), lastNonSpaceChar);
  input.remove_suffix(suffixLength);
  return input;
}

std::string toLower(std::string_view input) {
  std::string result;
  std::ranges::transform(
      input, std::back_inserter(result),
      [](const unsigned char ch) { return std::tolower(ch); });
  return result;
}

std::vector<std::string> split(std::string_view input, char delim) {
  std::vector<std::string> result;
  size_t current_delim = 0;
  size_t start = 0;
  size_t next = input.find(delim, start);
  while (current_delim != std::string_view::npos) {
    result.emplace_back(input.substr(start, next - start));
    current_delim = next;
    start = current_delim + 1;
    next = input.find(delim, start + 1);
  }
  return result;
}
} // namespace Lunacd::Util::Str
