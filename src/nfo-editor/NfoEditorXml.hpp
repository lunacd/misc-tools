#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace Lunacd::NfoEditor {
class Xml {
public:
  Xml(std::string originalFilename, std::string title, std::string studio,
      std::vector<std::string> actors, std::vector<std::string> tags)
      : filename(std::filesystem::path{originalFilename}
                     .replace_extension("nfo")
                     .filename()
                     .string()),
        title(std::move(title)), studio(std::move(studio)),
        actors(std::move(actors)), tags(std::move(tags)) {}

  std::string filename;
  std::string title;
  std::string studio;
  std::vector<std::string> actors{};
  std::vector<std::string> tags{};

  std::string exportToStr() const;
};
} // namespace Lunacd::NfoEditor