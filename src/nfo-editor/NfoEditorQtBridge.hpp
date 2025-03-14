#pragma once

#include <QObject>
#include <QQmlEngine>

namespace Lunacd::NfoEditor {
class QtBridge : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_NAMED_ELEMENT(NfoEditorQtBridge)
public:
  explicit QtBridge(QObject *parent = nullptr) : QObject(parent) {}
  Q_INVOKABLE static void saveToXml(const QUrl &filePath, const QString &title,
                                    const QString &studio,
                                    const QList<QString> &actors,
                                    const QList<QString> &tags);
};
} // namespace Lunacd::NfoEditor
