#include <ImageTransformerController.hpp>
#include <UtilOat.hpp>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/network/Server.hpp>

using namespace Lunacd;

void runServer() {
  // Create app components
  Util::Oat::AppComponent components;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Routes */
  router->addController(std::make_shared<ImageTransformer::Controller>());

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
