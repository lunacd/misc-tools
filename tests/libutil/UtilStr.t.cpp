#include <UtilStr.hpp>

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Lunacd::Util::Str {
namespace {
using namespace testing;

TEST(UtilStr, split) {
  // GIVEN
  std::string input = "a/b/c";

  // WHEN
  std::vector<std::string> result = split(input, '/');

  // THEN
  ASSERT_THAT(result, ElementsAre("a", "b", "c"));
}

TEST(UtilStr, splitEmptySegments) {
  // GIVEN
  std::string input = "a//////b//////c";

  // WHEN
  std::vector<std::string> result = split(input, '/');

  // THEN
  ASSERT_THAT(result, ElementsAre("a", "b", "c"));
}

TEST(UtilStr, splitLeadingAndTrailing) {
  // GIVEN
  std::string input = "//a/b/c//";

  // WHEN
  std::vector<std::string> result = split(input, '/');

  // THEN
  ASSERT_THAT(result, ElementsAre("a", "b", "c"));
}

} // namespace
} // namespace Lunacd::Util::Str