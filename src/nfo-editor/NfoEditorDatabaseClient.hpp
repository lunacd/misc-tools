#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(DbClient)

class DatabaseClient : public oatpp::orm::DbClient {
public:
  DatabaseClient(std::shared_ptr<oatpp::orm::Executor> executor)
      : oatpp::orm::DbClient(std::move(executor)) {}

  QUERY(getCompletions,
        "SELECT content, source FROM completions WHERE source=:source;",
        PARAM(oatpp::String, source));

  QUERY(addCompletion,
        "INSERT INTO completions (source, content) VALUES (:source, :content);",
        PARAM(oatpp::String, source), PARAM(oatpp::String, content));
};

#include OATPP_CODEGEN_END(DbClient)
} // namespace Lunacd::NfoEditor
