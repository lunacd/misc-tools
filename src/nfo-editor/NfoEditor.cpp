#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqml.h>

#include <NfoEditorQtBridge.hpp>

auto main(int argc, char **argv) -> int {
  QApplication app(argc, argv);

  qmlRegisterType<NfoEditor::QtBridge>("com.lunacd", 0, 1, "NfoEditorQtBridge");

  // Create bridge before the engine because they will be destructed in reverse
  // order.
  NfoEditor::QtBridge bridge;

  QQmlApplicationEngine engine;

  // Set bridge as a property of root context
  engine.rootContext()->setContextProperty("bridge", &bridge);

  engine.load(
      QUrl(QStringLiteral("qrc:/com/lunacd/nfoEditor/qml/NfoEditor.qml")));

  return QApplication::exec();
}