#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqml.h>

#include <NfoEditorQtBridge.hpp>

using namespace Lunacd;

auto main(int argc, char **argv) -> int {
  QApplication app(argc, argv);

  qmlRegisterType<NfoEditor::QtBridge>("com.lunacd.NfoEditorQtBridge", 0, 1,
                                       "NfoEditorQtBridge");

  // Create bridge before the engine because they will be destructed in reverse
  // order.
  NfoEditor::QtBridge bridge;

  QQmlApplicationEngine engine;

  // Set bridge as a property of root context
  engine.rootContext()->setContextProperty("bridge", &bridge);

  engine.load(QUrl(QStringLiteral("qrc:/qt/qml/nfo-editor/NfoEditor.qml")));

  return QApplication::exec();
}
