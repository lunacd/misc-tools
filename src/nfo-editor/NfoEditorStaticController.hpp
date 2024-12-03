#pragma once

#include <NfoEditorAutocomplete.hpp>
#include <NfoEditorXml.hpp>
#include <UtilFilesystem.hpp>
#include <UtilOat.hpp>
#include <UtilStr.hpp>
#include <dto/NfoEditorAutocompleteResponse.hpp>
#include <dto/NfoEditorSaveToNfoRequest.hpp>

#include <filesystem>
#include <mutex>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <semaphore.h>
#include <string>
#include <unordered_map>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(ApiController)

class StaticController : public oatpp::web::server::api::ApiController {
public:
  StaticController(std::filesystem::path staticRoot,
                   OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        m_staticRoot(std::move(staticRoot)) {}

  ADD_CORS(staticFiles, "*")
  ENDPOINT("GET", "/ui/*", staticFiles,
           REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    const std::string tail = request->getPathTail();
    const auto segments = Util::Str::split(tail, '/');
    auto targetPath = m_staticRoot;

    for (const auto &segment : segments) {
      targetPath = targetPath / segment;
    }
    if (!std::filesystem::is_regular_file(targetPath)) {
      return createResponse(Status::CODE_404, "Requested file not found");
    }

    if (!Util::Filesystem::pathIsContainedIn(targetPath, m_staticRoot)) {
      // Prevent traversal outside of root
      return createResponse(Status::CODE_404, "Requested file not found");
    }

    const auto mime = extToMIME(targetPath.extension());
    const auto fileContent = getFile(targetPath);

    auto response = createResponse(Status::CODE_200, fileContent);
    response->putHeader(Header::CONTENT_TYPE, mime);
    return response;
  }

private:
  const std::filesystem::path m_staticRoot;
  std::unordered_map<std::filesystem::path, std::string> m_cache;
  std::mutex m_cacheLock;

  std::string getFile(const std::filesystem::path &filePath);
  static std::string extToMIME(const std::string &ext);
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::NfoEditor