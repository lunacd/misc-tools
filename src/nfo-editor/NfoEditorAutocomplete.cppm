module;
#include <fmt/format.h>
#include <marisa/agent.h>
#include <marisa/keyset.h>
#include <marisa/trie.h>

export module lunacd.nfoEditor:autocomplete;

import std;
import lunacd.util;

namespace NfoEditor {
std::filesystem::path
getCompletionFilePath(const std::string &completionSource) {
  return SysUtil::getProgramDataDir("nfo-editor") / "autocomplete" /
         fmt::format("{}.txt", completionSource);
}

void exportCompletionToFile(
    const std::string &completionSource,
    const std::unordered_map<std::string, std::unordered_set<std::string>>
        &originalStrings) {
  const auto completionFilePath = getCompletionFilePath(completionSource);
  if (!std::filesystem::exists(completionFilePath.parent_path())) {
    std::filesystem::create_directories(completionFilePath.parent_path());
  }
  std::ofstream completionFile{completionFilePath};

  for (const auto &[matchStr, originalStrs] : originalStrings) {
    for (const auto &line : originalStrs) {
      completionFile << line << "\n";
    }
  }
}

export class Autocomplete {
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
    complete(const std::string &prefix) const {
      marisa::Agent agent;
      agent.set_query(prefix.c_str());
      std::vector<std::string> matches;
      matches.reserve(s_maxMatches);

      while (m_completionData->trie.predictive_search(agent) &&
             matches.size() < s_maxMatches) {
        const auto match =
            std::string_view(agent.key().ptr(), agent.key().length());
        const auto &originalStrings =
            m_completionData->originalStrings.at(std::string(match));
        matches.insert(matches.end(), originalStrings.begin(),
                       originalStrings.end());
      }

      return matches;
    }

    void addCandidate(const std::string &candidate) const {
      const auto candidateLower = StrUtil::toLower(candidate);
      m_completionData->keyset.push_back(candidateLower.c_str());
      m_completionData->trie.build(m_completionData->keyset);
      if (const auto it =
              m_completionData->originalStrings.find(candidateLower);
          it == m_completionData->originalStrings.end()) {
        m_completionData->originalStrings.emplace(
            candidateLower, std::unordered_set{candidate});
      } else {
        it->second.emplace(candidate);
      }
    }

  private:
    std::shared_ptr<CompletionData> m_completionData;
  };

  Autocomplete() = default;

  void registerCompletionSource(const std::string &completionSource) {
    getCompleter(completionSource);
  }

  Completer getCompleter(const std::string &completionSource) {
    auto it = m_completionData.find(completionSource);
    if (it == m_completionData.end()) {
      const auto completionData = buildCompletionData(completionSource);
      it = m_completionData.emplace(completionSource, completionData).first;
    }

    return Completer{it->second};
  }

  void exportCompletionData() const {
    for (const auto &[completionSourceName, completionData] :
         m_completionData) {
      exportCompletionToFile(completionSourceName,
                             completionData->originalStrings);
    }
  }

private:
  // Map from completion source name to the corresponding completion data
  std::unordered_map<std::string, std::shared_ptr<CompletionData>>
      m_completionData;

  static constexpr size_t s_maxMatches = 5;

  static std::shared_ptr<CompletionData>
  buildCompletionData(const std::string &completionSource) {
    auto completionData = std::make_shared<CompletionData>();

    // Check $XDG_DATA_HOME/nfo-editor/autocomplete/<source>.txt for completions
    const auto completionFilePath = getCompletionFilePath(completionSource);
    if (!std::filesystem::exists(completionFilePath)) {
      completionData->trie.build(completionData->keyset);
      return completionData;
    }

    std::ifstream completionFile{completionFilePath};
    std::string line;
    while (std::getline(completionFile, line)) {
      const auto candidate = std::string(StrUtil::trim(line));
      const auto candidateLower = StrUtil::toLower(candidate);
      if (candidate.empty()) {
        continue;
      }
      completionData->keyset.push_back(candidateLower.c_str());
      if (const auto it = completionData->originalStrings.find(candidateLower);
          it == completionData->originalStrings.end()) {
        completionData->originalStrings.emplace(candidateLower,
                                                std::unordered_set{candidate});
      } else {
        it->second.emplace(candidate);
      }
    }
    completionData->trie.build(completionData->keyset);

    return completionData;
  }
};
} // namespace NfoEditor