#pragma once

#include <OaRelayApiClient.hpp>
#include <OaRelayDatabaseClient.hpp>
#include <auth/OaRelayJwt.hpp>

#include <memory>

#include <oatpp-openssl/Config.hpp>
#include <oatpp-openssl/client/ConnectionProvider.hpp>
#include <oatpp-sqlite/orm.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

namespace Lunacd::OaRelay {
class AppComponent {
public:
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
        std::make_shared<oatpp::sqlite::ConnectionProvider>("./oaRelay.db");
    auto connectionPool = oatpp::sqlite::ConnectionPool::createShared(
        connectionProvider, 10, std::chrono::seconds(5));
    auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionPool);

    return std::make_shared<DatabaseClient>(executor);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<Jwt>, jwt)
  ([] { return std::make_shared<Jwt>(std::getenv("JWT_SECRET")); }());
};
} // namespace Lunacd::OaRelay
