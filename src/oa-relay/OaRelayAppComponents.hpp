#pragma once

#include <OaRelayApiClient.hpp>

#include <memory>

#include <oatpp-libressl/client/ConnectionProvider.hpp>
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
    auto config = oatpp::libressl::Config::createShared();
    tls_config_insecure_noverifycert(config->getTLSConfig());
    tls_config_insecure_noverifyname(config->getTLSConfig());
    return oatpp::libressl::client::ConnectionProvider::createShared(
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
};
} // namespace Lunacd::OaRelay
