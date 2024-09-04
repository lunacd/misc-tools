#include <NfoEditorAutocomplete.hpp>
#include <SysUtil.hpp>
#include <StrUtil.hpp>

#include <filesystem>
#include <fstream>
#include <marisa/key.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <fmt/format.h>
#include <marisa/agent.h>
#include <marisa/keyset.h>
#include <marisa/trie.h>

namespace NfoEditor {
namespace {
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
} // namespace

Autocomplete::Completer
Autocomplete::getCompleter(const std::string &completionSource) {
  auto it = m_completionData.find(completionSource);
  if (it == m_completionData.end()) {
    const auto completionData = buildCompletionData(completionSource);
    it = m_completionData.emplace(completionSource, completionData).first;
  }

  return Completer{it->second};
} // namespace NfoEditor

void Autocomplete::registerCompletionSource(
    const std::string &completionSource) {
  getCompleter(completionSource);
}

std::shared_ptr<Autocomplete::CompletionData>
Autocomplete::buildCompletionData(const std::string &completionSource) {
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

std::vector<std::string>
Autocomplete::Completer::complete(const std::string &prefix) const {
  marisa::Agent agent;
  agent.set_query(prefix.c_str());
  std::vector<std::string> matches;
  matches.reserve(s_maxMatches);

  while (m_completionData->trie.predictive_search(agent) &&
         matches.size() < s_maxMatches) {
    const auto *const match = agent.key().ptr();
    const auto &originalStrings =
        m_completionData->originalStrings.at(std::string(match));
    matches.insert(matches.end(), originalStrings.begin(),
                   originalStrings.end());
  }

  return matches;
}

void Autocomplete::Completer::addCandidate(const std::string &candidate) const {
  const auto candidateLower = StrUtil::toLower(candidate);
  m_completionData->keyset.push_back(candidateLower.c_str());
  m_completionData->trie.build(m_completionData->keyset);
  if (const auto it = m_completionData->originalStrings.find(candidateLower);
      it == m_completionData->originalStrings.end()) {
    m_completionData->originalStrings.emplace(candidateLower,
                                              std::unordered_set{candidate});
  } else {
    it->second.emplace(candidate);
  }
}

void Autocomplete::exportCompletionData() const {
  for (const auto &[completionSourceName, completionData] : m_completionData) {
    exportCompletionToFile(completionSourceName,
                           completionData->originalStrings);
  }
}
} // namespace NfoEditor