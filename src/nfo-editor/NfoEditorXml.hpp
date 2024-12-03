#pragma once

#include <string>
#include <vector>

namespace Lunacd::NfoEditor {
class Xml {
public:
  Xml(std::string title, std::string studio, std::vector<std::string> actors,
      std::vector<std::string> tags)
      : title(std::move(title)), studio(std::move(studio)),
        actors(std::move(actors)), tags(std::move(tags)) {}

  std::string title;
  std::string studio;
  std::vector<std::string> actors{};
  std::vector<std::string> tags{};

  void saveToFile(const std::string &filename) const;
};
} // namespace NfoEditor