#include <ImageTransformerQtBridge.hpp>

#include <filesystem>

#include <Magick++/Exception.h>
#include <Magick++/Functions.h>
#include <Magick++/Geometry.h>
#include <Magick++/Image.h>
#include <MagickCore/image.h>
#include <QFutureWatcher>
#include <QObject>
#include <QtConcurrent>

namespace Lunacd::ImageTransformer {
QtBridge::QtBridge(QObject *parent) : QObject(parent) {
  Magick::InitializeMagick(nullptr);
  connect(&m_futureWatcher, &QFutureWatcher<QString>::finished, this,
          &QtBridge::handleTransformFinished);
};

void QtBridge::transformImages(const QList<QUrl> &images, int targetWidth) {
  QFuture<QString> future = QtConcurrent::run([this, images, targetWidth]() {
    try {
      for (const auto &imageUrl : images) {
        std::filesystem::path imagePath{imageUrl.toLocalFile().toStdString()};
        Magick::Image image;
        image.read(imagePath.generic_string());
        image.strip();
        image.quality(95);
        const int currentWidth = image.columns();
        if (currentWidth > targetWidth) {
          image.resize(Magick::Geometry(std::format("{}x", targetWidth)));
        }

        const std::filesystem::path outputDir =
            imagePath.parent_path() / "converted";
        std::filesystem::create_directories(outputDir);
        const std::filesystem::path outputPath =
            outputDir / std::format("{}.webp", imagePath.stem().string());
        image.write(outputPath.generic_string());
      }
      return QString("Conversion successfully finished! See the converted "
                     "folder next to your input files.");
    } catch (const Magick::Exception &error) {
      return QString(error.what());
    }
  });
  m_futureWatcher.setFuture(future);
}

void QtBridge::handleTransformFinished() const {
  emit transformCompleted(m_futureWatcher.future().result());
}
} // namespace Lunacd::ImageTransformer
