#pragma once

#include <OaRelayApiClient.hpp>
#include <OaRelayDatabaseClient.hpp>
#include <auth/OaRelayJwt.hpp>
#include <interceptor/OaRelayAuthInterceptor.hpp>

#include <memory>

#include <oatpp-openssl/Config.hpp>
#include <oatpp-openssl/client/ConnectionProvider.hpp>
#include <oatpp-postgresql/orm.hpp>
#include <oatpp/core/async/Executor.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/AsyncHttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

namespace Lunacd::OaRelay {
class AppComponent {
public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<Jwt>, jwt)
  ([] { return std::make_shared<Jwt>(std::getenv("JWT_SECRET")); }());

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
   * Create async executor to execute coroutines.
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, asyncExecutor)
  ([] { return std::make_shared<oatpp::async::Executor>(); }());

  /**
   * Create ConnectionHandler component which uses Router component to route
   * requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                         serverConnectionHandler)
  ([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
    auto connectionHandler =
        oatpp::web::server::AsyncHttpConnectionHandler::createShared(router,
                                                                     executor);
    connectionHandler->addRequestInterceptor(
        std::make_shared<AuthInterceptor>());
    return connectionHandler;
  }());

  /**
   * Create ObjectMapper component to serialize/deserialize DTOs in
   * Controller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                         apiObjectMapper)
  ([] { return oatpp::parser::json::mapping::ObjectMapper::createShared(); }());
  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::network::ClientConnectionProvider>,
      sslClientConnectionProvider)
  ("clientConnectionProvider", [] {
    auto config = oatpp::openssl::Config::createDefaultClientConfigShared();
    return oatpp::openssl::client::ConnectionProvider::createShared(
        config, {"api.openai.com", 443});
  }());

  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::web::client::HttpRequestExecutor>, requestExecutor)
  ([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
                    connectionProvider, "clientConnectionProvider");
    return oatpp::web::client::HttpRequestExecutor::createShared(
        connectionProvider);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<ApiClient>, myApiClient)
  ([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
                    connectionProvider, "clientConnectionProvider");
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                    objectMapper);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::client::HttpRequestExecutor>,
                    requestExecutor);
    return ApiClient::createShared(requestExecutor, objectMapper);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<DatabaseClient>, databaseClient)
  ([] {
    auto connectionProvider =
        std::make_shared<oatpp::postgresql::ConnectionProvider>(std::getenv("OA_RELAY_POSTGRESQL_URL"));
    auto connectionPool = oatpp::postgresql::ConnectionPool::createShared(
        connectionProvider, 10, std::chrono::seconds(5));
    auto executor = std::make_shared<oatpp::postgresql::Executor>(connectionPool);

    return std::make_shared<DatabaseClient>(executor);
  }());
};
} // namespace Lunacd::OaRelay
