#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <marisa/keyset.h>
#include <marisa/trie.h>

namespace NfoEditor {
class Autocomplete {
public:
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

    void addCandidate(const std::string &candidate) const;

  private:
    std::shared_ptr<CompletionData> m_completionData;
  };

  Autocomplete() = default;

  void registerCompletionSource(const std::string &completionSource);

  Completer getCompleter(const std::string &completionSource);

  void exportCompletionData() const;

private:
  // Map from completion source name to the corresponding completion data
  std::unordered_map<std::string, std::shared_ptr<CompletionData>>
      m_completionData;

  static constexpr size_t s_maxMatches = 5;

  static std::shared_ptr<CompletionData>
  buildCompletionData(const std::string &completionSource);
};
} // namespace NfoEditor