#include <NfoEditorQtBridge.hpp>

#include <iterator>

#include <NfoEditorXml.hpp>
#include <UtilStr.hpp>

#include <QUrl>

namespace NfoEditor {
void QtBridge::saveToXml(const QUrl &filePath, const QString &title,
                         const QString &studio, const QList<QString> &actors,
                         const QList<QString> &tags) {
  static const auto qStringToStd = [](const auto &actor) {
    return actor.toStdString();
  };

  std::vector<std::string> stdActors;
  stdActors.reserve(actors.size());
  std::vector<std::string> stdTags;
  stdTags.reserve(tags.size());
  std::ranges::transform(actors, std::back_inserter(stdActors), qStringToStd);
  std::ranges::transform(tags, std::back_inserter(stdTags), qStringToStd);

  const Xml data{title.toStdString(), studio.toStdString(), stdActors, stdTags};
  data.saveToFile(filePath.toLocalFile().toStdWString());
}
} // namespace NfoEditor