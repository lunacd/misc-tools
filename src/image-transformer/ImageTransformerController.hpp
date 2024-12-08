#pragma once

#include "dto/ImageTransformerConvertImageResponse.hpp"
#include <UtilExpiringResource.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <Magick++.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fmt/format.h>
#include <oatpp/core/data/resource/TemporaryFile.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <unordered_map>

namespace Lunacd::ImageTransformer {
#include OATPP_CODEGEN_BEGIN(ApiController)

namespace {} // namespace

class Controller : public oatpp::web::server::api::ApiController {
public:
  Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        m_fileDir(std::filesystem::current_path() / "files"),
        m_cachedImagesExpiring(1200, std::bind(&Controller::cleanupImages, this,
                                               std::placeholders::_1)) {}

  ENDPOINT("POST", "/imageTransformer/convert", convert,
           REQUEST(std::shared_ptr<IncomingRequest>, request),
           QUERY(String, filename), QUERY(Int32, targetWidth)) {
    oatpp::data::resource::TemporaryFile tmp("/tmp");
    request->transferBodyToStream(tmp.openOutputStream());
    std::string uuid;
    std::filesystem::path filenamePath{filename};
    std::string extension = filenamePath.extension().string();
    std::string stem = filenamePath.stem().string();
    {
      std::lock_guard<std::mutex> lock{m_uuidGeneratorLock};
      uuid = to_string(m_uuidGenerator()) + extension;
    }

    // Save file to file system
    if (!std::filesystem::exists(m_fileDir)) {
      std::filesystem::create_directory(m_fileDir);
    }
    std::filesystem::path saveFilePath = m_fileDir / uuid;
    tmp.moveFile(saveFilePath.string());
    {
      std::lock_guard<std::mutex> lock{m_cachedImagesLock};
      m_cachedImagesStatus.emplace(uuid, ImageConversionStatus{stem, false});
      m_cachedImagesExpiring.insert(uuid);
    }

    // Start conversion on a different thread
    std::thread conversionThread{&Controller::convertImage, this, saveFilePath,
                                 targetWidth};
    conversionThread.detach();

    // Return uuid for downloading converted images
    auto dto = ImageTransformerConvertImageResponse::createShared();
    dto->uuid = uuid;

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/imageTransformer/status", status, QUERY(String, uuid)) {
    std::lock_guard<std::mutex> lock{m_cachedImagesLock};
    const auto it = m_cachedImagesStatus.find(uuid);
    // Look for in-progress job
    if (it == m_cachedImagesStatus.end()) {
      return createResponse(Status::CODE_404, "Requested image not found");
    }
    // Check for completion status
    const auto finished = it->second.finished;
    if (!finished) {
      return createResponse(Status::CODE_202, "Conversion still in progress");
    }
    return createResponse(Status::CODE_200, "File ready to download");
  }

  ENDPOINT("GET", "/imageTransformer/download", download, QUERY(String, uuid)) {
    std::lock_guard<std::mutex> lock{m_cachedImagesLock};
    const auto it = m_cachedImagesStatus.find(uuid);
    // Look for in-progress job
    if (it == m_cachedImagesStatus.end()) {
      return createResponse(Status::CODE_404, "Requested image not found");
    }
    // Check for completion status
    const auto finished = it->second.finished;
    const auto convertedFilePath =
        m_fileDir / getConvertedFilename(std::filesystem::path(uuid));
    if (!finished || !std::filesystem::exists(convertedFilePath)) {
      return createResponse(Status::CODE_202, "Conversion still in progress");
    }
    // Converted image available, returning
    auto fileStem = it->second.filestem;
    auto body = oatpp::String::loadFromFile(convertedFilePath.c_str());
    auto response = createResponse(Status::CODE_200, body);
    response->putHeader("Content-Disposition",
                        std::format("attachment; filename={}.webp", fileStem));
    m_cachedImagesStatus.erase(it);
    m_cachedImagesExpiring.erase(uuid);
    return response;
  }

private:
  struct ImageConversionStatus {
    std::string filestem;
    bool finished;
  };

  boost::uuids::random_generator m_uuidGenerator;
  std::mutex m_uuidGeneratorLock;
  const std::filesystem::path m_fileDir;
  Util::ExpiringResource::ExpiringResource<std::string> m_cachedImagesExpiring;
  std::unordered_map<std::string, ImageConversionStatus> m_cachedImagesStatus;
  std::mutex m_cachedImagesLock;

  static std::filesystem::path
  getConvertedFilename(const std::filesystem::path &originalFilename) {
    auto newFilename = originalFilename;
    return newFilename
        .replace_filename(
            std::format("{}-converted", originalFilename.filename().string()))
        .replace_extension("webp");
  }

  void cleanupImages(const std::string &uuid) {
    std::filesystem::path originalPath = m_fileDir / uuid;
    std::filesystem::path convertedPath =
        m_fileDir / getConvertedFilename(uuid);
    if (std::filesystem::is_regular_file(originalPath)) {
      std::filesystem::remove(originalPath);
    }
    if (std::filesystem::is_regular_file(convertedPath)) {
      std::filesystem::remove(convertedPath);
    }
  }

  void convertImage(std::filesystem::path imagePath, int targetWidth) {
    Magick::Image image;
    image.read(imagePath.generic_string());
    image.strip();
    image.quality(95);
    const int currentWidth = image.columns();
    if (currentWidth > targetWidth) {
      image.resize(Magick::Geometry(fmt::format("{}x", targetWidth)));
    }
    image.defineValue("webp", "lossless", "true");

    const std::filesystem::path outputPath =
        m_fileDir / getConvertedFilename(imagePath.filename());
    image.write(outputPath.generic_string());
    {
      std::lock_guard<std::mutex> lock{m_cachedImagesLock};
      const auto it = m_cachedImagesStatus.find(imagePath.filename().string());
      if (it == m_cachedImagesStatus.end()) {
        return;
      }
      it->second.finished = true;
    }
  }
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::ImageTransformer
