#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqml.h>

#include <ImageTransformerQtBridge.hpp>

auto main(int argc, char **argv) -> int {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  engine.load(QUrl(QStringLiteral(
      "qrc:/com/lunacd/imageTransformer/qml/ImageTransformer.qml")));

  return QApplication::exec();
}
