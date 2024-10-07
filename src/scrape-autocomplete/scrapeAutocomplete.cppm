#include <fmt/format.h>
#include <fmt/xchar.h>
#include <pugixml.hpp>

import lunacd.util;
import std;

namespace ScrapeAutocomplete {
class Scraper {
public:
  void load(const std::filesystem::path &path) {
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

  void save() const {
    saveFile("studio", studios);
    saveFile("actor", actors);
    saveFile("tag", tags);
  }

private:
  std::unordered_set<std::string> studios;
  std::unordered_set<std::string> actors;
  std::unordered_set<std::string> tags;

  static void saveFile(const std::string_view completionSource,
                       const std::unordered_set<std::string> &data) {
    const auto completionFilePath = getCompletionFilePath(completionSource);
    if (!exists(completionFilePath.parent_path())) {
      create_directories(completionFilePath.parent_path());
    }
    std::ofstream outFile{completionFilePath};
    std::cout << getCompletionFilePath(completionSource).string() << std::endl;
    for (const auto &item : data) {
      outFile << item << '\n';
    }
  }

  [[nodiscard]] static std::filesystem::path
  getCompletionFilePath(std::string_view completionSource) {
    return Util::Sys::getProgramDataDir("nfo-editor") / "autocomplete" /
           fmt::format("{}.txt", completionSource);
  }
};
} // namespace ScrapeAutocomplete

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "scrape-autocomplete [directory to scrape]\n";
  }

  // Descend into directory, looking for .nfo files
  ScrapeAutocomplete::Scraper scraper;
  for (const std::filesystem::path directory = argv[1];
       const auto &dir_entry :
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
