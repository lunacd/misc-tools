#pragma once

#include <NfoEditorDatabaseClient.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <marisa/keyset.h>
#include <marisa/trie.h>
#include <oatpp/core/macro/component.hpp>

namespace Lunacd::NfoEditor {
class Autocomplete {
public:
  Autocomplete(OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, databaseClient))
      : m_databaseClient{std::move(databaseClient)} {}

  void registerCompletionSource(const std::string &completionSource);
  std::vector<std::string> autocomplete(const std::string &completionSource,
                                        const std::string &prefix);
  void addCompletionCandidate(const std::string &completionSource,
                              const std::string &candidate);

  struct CompletionData {
    marisa::Trie trie;
    marisa::Keyset keyset;
    std::unordered_map<std::string, std::unordered_set<std::string>>
        originalStrings;
  };

  class Completer {
  public:
    explicit Completer(std::shared_ptr<CompletionData> completionData)
        : m_completionData{std::move(completionData)} {}

    [[nodiscard]] std::vector<std::string>
    complete(const std::string &prefix) const;

  private:
    void addCandidate(const std::string &candidate) const;
    std::shared_ptr<CompletionData> m_completionData;

    friend class Autocomplete;
  };

  Completer getCompleter(const std::string &completionSource);

private:
  // Map from completion source name to the corresponding completion data
  std::unordered_map<std::string, std::shared_ptr<CompletionData>>
      m_completionData;
  static constexpr size_t s_maxMatches = 10;
  std::shared_ptr<DatabaseClient> m_databaseClient;

  std::shared_ptr<CompletionData>
  buildCompletionData(const std::string &completionSource);
};
} // namespace Lunacd::NfoEditor
