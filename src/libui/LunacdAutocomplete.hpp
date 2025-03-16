#pragma once

#include <memory>
#include <qobject.h>
#include <qtmetamacros.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <QObject>
#include <QQmlEngine>
#include <marisa/keyset.h>
#include <marisa/trie.h>
#include <qqml.h>

namespace Lunacd::UI {
class Autocomplete : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(LunacdAutocomplete)
  QML_SINGLETON
public:
  Autocomplete(QObject *parent = nullptr) : QObject(parent) {}

  Q_INVOKABLE void registerCompletionSource(const QString &completionSource);
  Q_INVOKABLE QList<QString> autocomplete(const QString &completionSource,
                                          const QString &prefix);
  Q_INVOKABLE void addCompletionCandidate(const QString &completionSource,
                                          const QString &candidate);
  Q_INVOKABLE void exportCompletionData() const;

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

  Completer getCompleter(const std::string &completionSource);

private:
  // Map from completion source name to the corresponding completion data
  std::unordered_map<std::string, std::shared_ptr<CompletionData>>
      m_completionData;

  static constexpr size_t s_maxMatches = 5;

  static std::shared_ptr<CompletionData>
  buildCompletionData(const std::string &completionSource);
};
} // namespace Lunacd::UI
