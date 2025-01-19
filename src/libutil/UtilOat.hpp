#pragma once

#include <UtilFilesystem.hpp>
#include <UtilStr.hpp>

#include <iterator>
#include <memory>
#include <optional>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/async/Executor.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>
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
