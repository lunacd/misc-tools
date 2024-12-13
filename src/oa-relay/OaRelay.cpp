
#include <OaRelayAppComponents.hpp>
#include <OaRelayController.hpp>
#include <UtilOat.hpp>

#include <memory>

#include <oatpp-libressl/Callbacks.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

using namespace Lunacd;

void runServer() {
  // Set lockingCallback for libressl
  oatpp::libressl::Callbacks::setDefaultCallbacks();

  // Create app components
  Util::Oat::AppComponent components;
  OaRelay::AppComponent oaRelayComponents;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Routes */
  router->addController(std::make_shared<OaRelay::Controller>());
  router->addController(std::make_shared<Util::Oat::StaticController>(
      std::filesystem::current_path() / "ui"));

  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                  connectionHandler);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                  connectionProvider);

  oatpp::network::Server server(connectionProvider, connectionHandler);

  server.run();
}

auto main(int argc, char **argv) -> int {
  oatpp::base::Environment::init();

  runServer();

  oatpp::base::Environment::destroy();
}
