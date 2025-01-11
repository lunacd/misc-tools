
#include <OaRelayAppComponents.hpp>
#include <OaRelayController.hpp>
#include <UtilOat.hpp>

#include <memory>

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

using namespace Lunacd;

void runServer() {
  // Create app components
  OaRelay::AppComponent oaRelayComponents;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Routes */
  router->addController(std::make_shared<OaRelay::Controller>());
  router->addController(std::make_shared<Util::Oat::AsyncStaticController>(
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
