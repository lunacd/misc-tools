#pragma once

#include <QObject>

import lunacd.nfoEditor;

namespace NfoEditor {
class QtBridge : public QObject {
  Q_OBJECT
public:
  explicit QtBridge(QObject *parent = nullptr) : QObject(parent) {}
  Q_INVOKABLE void registerCompletionSource(const QString &completionSource);
  Q_INVOKABLE QList<QString> autocomplete(const QString &completionSource,
                                          const QString &prefix);
  Q_INVOKABLE void addCompletionCandidate(const QString &completionSource,
                                          const QString &candidate);
  Q_INVOKABLE void exportCompletionData() const;
  Q_INVOKABLE static void saveToXml(const QUrl &filePath, const QString &title,
                                    const QString &studio,
                                    const QList<QString> &actors,
                                    const QList<QString> &tags);

private:
  Autocomplete m_autocomplete;
};
} // namespace NfoEditor