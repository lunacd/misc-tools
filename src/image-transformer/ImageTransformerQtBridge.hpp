#pragma once

#include <qfuturewatcher.h>
#include <qlist.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qurl.h>

namespace ImageTransformer {
class QtBridge : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(ImageTransformerQtBridge)
  QML_SINGLETON
public:
  explicit QtBridge(QObject *parent = nullptr);

  Q_INVOKABLE void transformImages(const QList<QUrl> &images, int targetWidth);
signals:
  void transformCompleted() const;

private slots:
  void handleTransformFinished() const;

private:
  QFutureWatcher<void> m_futureWatcher;
};
} // namespace ImageTransformer
