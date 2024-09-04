#include <StrUtil.hpp>

#include <algorithm>
#include <iterator>

namespace StrUtil {
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
} // namespace StrUtil
