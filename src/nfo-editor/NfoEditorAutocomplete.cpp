#include <NfoEditorAutocomplete.hpp>
#include <NfoEditorDtos.hpp>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <unordered_map>
#include <unordered_set>

#include <UtilStr.hpp>

#include <fmt/format.h>
#include <marisa/agent.h>
#include <marisa/keyset.h>
#include <marisa/trie.h>

namespace Lunacd::NfoEditor {
void Autocomplete::registerCompletionSource(
    const std::string &completionSource) {
  getCompleter(completionSource);
}

std::vector<std::string>
Autocomplete::autocomplete(const std::string &completionSource,
                           const std::string &prefix) {
  // Convert prefix to lowercase before matching
  const auto lowerPrefix = Util::Str::toLower(prefix);

  // Query backend for completion
  const auto completer = getCompleter(completionSource);
  const auto matches = completer.complete(lowerPrefix);
  std::vector<std::string> qResult;
  for (const auto &match : matches) {
    qResult.emplace_back(match);
  }
  return qResult;
}

void Autocomplete::addCompletionCandidate(const std::string &completionSource,
                                          const std::string &candidate) {
  const auto completer = getCompleter(completionSource);
  completer.addCandidate(candidate);
  m_databaseClient->addCompletion(completionSource, candidate);
}

std::vector<std::string>
Autocomplete::Completer::complete(const std::string &prefix) const {
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

void Autocomplete::Completer::addCandidate(const std::string &candidate) const {
  const auto candidateLower = Util::Str::toLower(candidate);
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

Autocomplete::Completer
Autocomplete::getCompleter(const std::string &completionSource) {
  auto it = m_completionData.find(completionSource);
  if (it == m_completionData.end()) {
    const auto completionData = buildCompletionData(completionSource);
    it = m_completionData.emplace(completionSource, completionData).first;
  }

  return Completer{it->second};
}

std::shared_ptr<Autocomplete::CompletionData>
Autocomplete::buildCompletionData(const std::string &completionSource) {
  auto completionData = std::make_shared<CompletionData>();

  OATPP_LOGI("NfoEditor", "Loading completion source %s from database.",
             completionSource.c_str());

  const auto queryResult = m_databaseClient->getCompletions(completionSource);
  if (!queryResult->isSuccess()) {
    OATPP_LOGE("NfoEditor",
               "Failed to load completion source %s from database, error: %s.",
               completionSource.c_str(),
               queryResult->getErrorMessage()->c_str());
    return completionData;
  }

  const auto completions =
      queryResult->fetch<oatpp::Vector<oatpp::Object<DbCompletionRow>>>();
  for (const auto &completion : *completions) {
    const auto candidateLower =
        Util::Str::toLower(completion->content->c_str());
    completionData->keyset.push_back(candidateLower.c_str());
    if (const auto it = completionData->originalStrings.find(candidateLower);
        it == completionData->originalStrings.end()) {
      completionData->originalStrings.emplace(
          candidateLower, std::unordered_set<std::string>{completion->content});
    } else {
      it->second.emplace(completion->content);
    }
  }

  completionData->trie.build(completionData->keyset);

  return completionData;
}
} // namespace Lunacd::NfoEditor