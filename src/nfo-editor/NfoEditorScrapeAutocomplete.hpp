#pragma once

#include <NfoEditorDatabaseClient.hpp>

#include <filesystem>
#include <memory>
#include <unordered_set>

#include <oatpp/core/macro/component.hpp>
#include <pugixml.hpp>

namespace Lunacd::NfoEditor {
class Scraper {
public:
  Scraper(OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, databaseClient))
      : m_databaseClient{std::move(databaseClient)} {}

  void load(const std::filesystem::path &path);

  void save() const;

private:
  std::unordered_set<std::string> studios;
  std::unordered_set<std::string> actors;
  std::unordered_set<std::string> tags;
  std::shared_ptr<DatabaseClient> m_databaseClient;

  void saveToDatabase(const std::string_view completionSource,
                             const std::unordered_set<std::string> &data) const;
};

void scrapeAutocomplete(const std::filesystem::path &directory);
} // namespace Lunacd::NfoEditor
