#pragma once

#include <QObject>

namespace NfoEditor {
class QtBridge : public QObject {
  Q_OBJECT
public:
  explicit QtBridge(QObject *parent = nullptr) : QObject(parent) {}
  Q_INVOKABLE static void saveToXml(const QUrl &filePath, const QString &title,
                                    const QString &studio,
                                    const QList<QString> &actors,
                                    const QList<QString> &tags);
};
} // namespace NfoEditor