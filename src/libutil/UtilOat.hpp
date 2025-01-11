#pragma once

#include <UtilFilesystem.hpp>
#include <UtilStr.hpp>

#include <filesystem>
#include <iterator>

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/async/Executor.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <optional>

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
        {"0.0.0.0", 8000, oatpp::network::Address::IP_4});
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

class StaticControllerBase {
public:
  StaticControllerBase(std::filesystem::path staticRoot)
      : m_staticRoot(std::move(staticRoot)) {}

protected:
  const std::filesystem::path m_staticRoot;
  std::unordered_map<std::filesystem::path, std::string> m_cache;
  std::mutex m_cacheLock;

  std::string getFile(const std::filesystem::path &filePath);
  static std::string extToMIME(const std::string &ext);
};

#include OATPP_CODEGEN_BEGIN(ApiController)

class StaticController : public oatpp::web::server::api::ApiController,
                         public StaticControllerBase {
public:
  StaticController(std::filesystem::path staticRoot,
                   OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        StaticControllerBase(std::move(staticRoot)) {}

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
};

#include OATPP_CODEGEN_END(ApiController)

#include OATPP_CODEGEN_BEGIN(ApiController)

class AsyncStaticController : public oatpp::web::server::api::ApiController,
                              public StaticControllerBase {
public:
  typedef AsyncStaticController __ControllerType;
  AsyncStaticController(std::filesystem::path staticRoot,
                        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                        objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        StaticControllerBase(std::move(staticRoot)) {}

  friend class StaticFilesAsync;

  ENDPOINT_ASYNC("GET", "/ui/*", StaticFilesAsync) {
    ENDPOINT_ASYNC_INIT(StaticFilesAsync);

    Action act() {
      const std::string tail = request->getPathTail();
      const auto segments = Util::Str::split(tail, '/');
      auto targetPath = controller->m_staticRoot;

      for (const auto &segment : segments) {
        targetPath = targetPath / segment;
      }
      if (!std::filesystem::is_regular_file(targetPath)) {
        return _return(controller->createResponse(Status::CODE_404,
                                                  "Requested file not found"));
      }

      if (!Util::Filesystem::pathIsContainedIn(targetPath,
                                               controller->m_staticRoot)) {
        // Prevent traversal outside of root
        return _return(controller->createResponse(Status::CODE_404,
                                                  "Requested file not found"));
      }

      const auto mime = extToMIME(targetPath.extension());
      const auto fileContent = controller->getFile(targetPath);

      auto response = controller->createResponse(Status::CODE_200, fileContent);
      response->putHeader(Header::CONTENT_TYPE, mime);
      return _return(response);
    }
  };
};

#include OATPP_CODEGEN_END(ApiController)

template <typename T1, typename T2>
std::vector<T1> oatppVectorToStdVector(const oatpp::Vector<T2> &oatppVec) {
  std::vector<T1> stdVec;
  stdVec.reserve(oatppVec->size());
  std::copy(oatppVec->begin(), oatppVec->end(), std::back_inserter(stdVec));
  return stdVec;
}

std::optional<std::string>
getCookie(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request>
              &request,
          std::string_view cookieName);
} // namespace Lunacd::Util::Oat