#include "gmock/gmock.h"
#include <UtilFilesystem.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Lunacd::Util::Filesystem {
namespace {
using namespace testing;

TEST(UtilFilesystem, contains) {
  // GIVEN
  const auto pathA = std::filesystem::path("/a/");
  const auto pathB = std::filesystem::path("/a/b/");
  const auto pathC = std::filesystem::path("/a/b/c/");

  // THEN
  ASSERT_THAT(pathIsContainedIn(pathB, pathA), IsTrue());
  ASSERT_THAT(pathIsContainedIn(pathB, pathC), IsFalse());
  ASSERT_THAT(pathIsContainedIn(pathA, pathC), IsFalse());
}
} // namespace
} // namespace Lunacd::Util::Filesystem
