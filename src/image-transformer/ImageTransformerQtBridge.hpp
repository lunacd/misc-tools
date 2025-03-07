#pragma once

#include <QFutureWatcher>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QUrl>

namespace Lunacd::ImageTransformer {
class QtBridge : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_NAMED_ELEMENT(ImageTransformerQtBridge)
public:
  explicit QtBridge(QObject *parent = nullptr);

  Q_INVOKABLE void transformImages(const QList<QUrl> &images, int targetWidth);
signals:
  void transformCompleted(const QString &message) const;

private slots:
  void handleTransformFinished() const;

private:
  QFutureWatcher<QString> m_futureWatcher;
};
} // namespace Lunacd::ImageTransformer
