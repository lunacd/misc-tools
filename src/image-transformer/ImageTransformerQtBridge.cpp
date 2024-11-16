#include <ImageTransformerQtBridge.hpp>

#include <filesystem>

#include <Magick++/Functions.h>
#include <Magick++/Geometry.h>
#include <Magick++/Image.h>
#include <MagickCore/image.h>
#include <QFutureWatcher>
#include <QObject>
#include <QtConcurrent>
#include <fmt/format.h>

namespace ImageTransformer {
QtBridge::QtBridge(QObject *parent) : QObject(parent) {
  Magick::InitializeMagick(nullptr);
  connect(&m_futureWatcher, &QFutureWatcher<void>::finished, this,
          &QtBridge::handleTransformFinished);
};

void QtBridge::transformImages(const QList<QUrl> &images, int targetWidth) {
  QFuture<void> future = QtConcurrent::run([this, images, targetWidth]() {
    for (const auto &imageUrl : images) {
      std::filesystem::path imagePath = imageUrl.toLocalFile().toStdString();
      Magick::Image image;
      image.read(imagePath.generic_string());
      image.strip();
      image.quality(95);
      const int currentWidth = image.columns();
      if (currentWidth > targetWidth) {
        image.resize(Magick::Geometry(fmt::format("{}x", targetWidth)));
      }
      image.defineValue("webp", "lossless", "true");

      const std::filesystem::path outputDir =
          imagePath.parent_path() / "converted";
      std::filesystem::create_directories(outputDir);
      const std::filesystem::path outputPath =
          outputDir / fmt::format("{}.webp", imagePath.stem().string());
      image.write(outputPath.generic_string());
    }
  });
  m_futureWatcher.setFuture(future);
}

void QtBridge::handleTransformFinished() const { emit transformCompleted(); }
} // namespace ImageTransformer