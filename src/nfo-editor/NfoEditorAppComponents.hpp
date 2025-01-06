#pragma once

#include <NfoEditorDatabaseClient.hpp>

#include <cstdlib>
#include <memory>

#include <oatpp-postgresql/orm.hpp>
#include <oatpp/core/macro/component.hpp>

namespace Lunacd::NfoEditor {
class AppComponent {
public:
  OATPP_CREATE_COMPONENT(std::shared_ptr<DatabaseClient>, databaseClient)
  ([] {
    auto connectionProvider =
        std::make_shared<oatpp::postgresql::ConnectionProvider>(
            std::getenv("NFO_EDITOR_POSTGRESQL_URL"));
    auto connectionPool = oatpp::postgresql::ConnectionPool::createShared(
        connectionProvider, 10, std::chrono::seconds(5));
    auto executor =
        std::make_shared<oatpp::postgresql::Executor>(connectionPool);

    return std::make_shared<DatabaseClient>(executor);
  }());
};
} // namespace Lunacd::NfoEditor
