#include <ImageTransformerQtBridge.hpp>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

using namespace Lunacd::ImageTransformer;

auto main(int argc, char **argv) -> int {
  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  engine.load(QUrl(
      QStringLiteral("qrc:/qt/qml/ImageTransformer/ImageTransformer.qml")));

  return QApplication::exec();
}
