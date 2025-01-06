#include <NfoEditorScrapeAutocomplete.hpp>

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_set>

#include <fmt/format.h>
#include <fmt/xchar.h>
#include <pugixml.hpp>

namespace Lunacd::NfoEditor {
void Scraper::load(const std::filesystem::path &path) {
  pugi::xml_document doc;
  if (const pugi::xml_parse_result result =
          doc.load_file(path.wstring().c_str());
      !result) {
    throw std::runtime_error(result.description());
  }
  const auto root = doc.first_child();
  if (std::string(root.name()) != "movie") {
    throw std::runtime_error("ScrapeAutocomplete only supports movies");
  }

  for (const auto &child : root.children()) {
    std::string nodeName = child.name();
    if (nodeName == "studio") {
      studios.emplace(child.child_value());
      continue;
    }
    if (nodeName == "tag") {
      tags.emplace(child.child_value());
      continue;
    }
    if (nodeName == "actor") {
      actors.emplace(child.child("name").child_value());
    }
  }
}

void Scraper::save() const {
  saveToDatabase("studio", studios);
  saveToDatabase("actor", actors);
  saveToDatabase("tag", tags);
}

void Scraper::saveToDatabase(
    const std::string_view completionSource,
    const std::unordered_set<std::string> &data) const {
  for (const auto &item : data) {
    const auto result =
        m_databaseClient->addCompletion(std::string(completionSource), item);
    if (!result->isSuccess()) {
      std::cout << result->getErrorMessage()->c_str() << "\n";
    }
  }
}

void scrapeAutocomplete(const std::filesystem::path &directory) {
  // Descend into directory, looking for .nfo files
  Scraper scraper;
  for (const auto &dir_entry :
       std::filesystem::recursive_directory_iterator(directory)) {
    const auto &path = dir_entry.path();
    auto absolutePath = absolute(path);
    if (!is_regular_file(dir_entry)) {
      std::wcout << fmt::format(L"Skipping directory {}\n",
                                absolutePath.wstring());
      continue;
    }
    if (path.extension() != ".nfo") {
      std::wcout << fmt::format(L"Skipping non-nfo {}\n",
                                absolutePath.wstring());
      continue;
    }
    std::wcout << fmt::format(L"Loading {}\n", absolutePath.wstring());
    scraper.load(path);
  }

  // Save scraped data
  scraper.save();
}
} // namespace Lunacd::NfoEditor
