#pragma once

#include <UtilFilesystem.hpp>
#include <UtilStr.hpp>

#include <filesystem>
#include <iterator>

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace Lunacd::Util::Oat {
class AppComponent {
public:
  /**
   * Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      serverConnectionProvider)
  ("serverConnectionProvider", [] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared(
        {"localhost", 8000, oatpp::network::Address::IP_4});
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                         httpRouter)
  ([] { return oatpp::web::server::HttpRouter::createShared(); }());

  /**
   * Create ConnectionHandler component which uses Router component to route
   * requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                         serverConnectionHandler)
  ([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                    router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  /**
   * Create ObjectMapper component to serialize/deserialize DTOs in
   * Controller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         apiObjectMapper)
  ([] { return oatpp::parser::json::mapping::ObjectMapper::createShared(); }());
};

#include OATPP_CODEGEN_BEGIN(ApiController)

class StaticController : public oatpp::web::server::api::ApiController {
public:
  StaticController(std::filesystem::path staticRoot,
                   OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        m_staticRoot(std::move(staticRoot)) {}

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

template <typename T1, typename T2>
std::vector<T1> oatppVectorToStdVector(const oatpp::Vector<T2> &oatppVec) {
  std::vector<T1> stdVec;
  stdVec.reserve(oatppVec->size());
  std::copy(oatppVec->begin(), oatppVec->end(), std::back_inserter(stdVec));
  return stdVec;
}
} // namespace Lunacd::Util::Oat