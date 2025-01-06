#include <NfoEditorAppComponents.hpp>
#include <NfoEditorController.hpp>
#include <NfoEditorScrapeAutocomplete.hpp>
#include <UtilOat.hpp>

#include <memory>

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

using namespace Lunacd;

void scrape(const std::filesystem::path &directory) {
  NfoEditor::AppComponent nfoEditorComponents;
  NfoEditor::scrapeAutocomplete(directory);
}

void runServer() {
  // Create app components
  Util::Oat::AppComponent components;
  NfoEditor::AppComponent nfoEditorComponents;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Routes */
  router->addController(std::make_shared<NfoEditor::Controller>());
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

  if (argc > 2 && strcmp(argv[1], "scrape") == 0) {
    scrape(std::filesystem::path{argv[2]});
  } else {
    runServer();
  }

  oatpp::base::Environment::destroy();
}