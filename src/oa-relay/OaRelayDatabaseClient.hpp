#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/orm/DbClient.hpp>
#include <oatpp/orm/SchemaMigration.hpp>

namespace Lunacd::OaRelay {
#include OATPP_CODEGEN_BEGIN(DbClient)

class DatabaseClient : public oatpp::orm::DbClient {
public:
  DatabaseClient(std::shared_ptr<oatpp::orm::Executor> executor)
      : oatpp::orm::DbClient(std::move(executor)) {}

  QUERY(createUser,
        "INSERT INTO users (username, password) VALUES (:username, :password);",
        PARAM(oatpp::String, username), PARAM(oatpp::String, password));

  QUERY(
      getUserPassword,
      "SELECT user_id, username, password FROM users WHERE username=:username;",
      PARAM(oatpp::String, username));

  QUERY(getMessages,
        "SELECT role, message AS content FROM messages "
        "JOIN sessions ON sessions.session_row_id = messages.session_row_id "
        "WHERE session_id=:session_id "
        "ORDER BY msg_order;",
        PARAM(oatpp::Int32, session_id));

  QUERY(newSession,
        "INSERT INTO sessions (session_id, user_id) VALUES (:session_id, "
        ":user_id)",
        PARAM(oatpp::Int32, user_id), PARAM(oatpp::Int32, session_id));

  QUERY(newMessage,
        "INSERT INTO messages (session_row_id, role, message, msg_order) "
        "VALUES ("
        "(SELECT session_row_id FROM sessions "
        "WHERE user_id=:user_id AND session_id=:session_id), "
        ":role, :content, :order);",
        PARAM(oatpp::Int32, user_id), PARAM(oatpp::Int32, session_id),
        PARAM(oatpp::String, role), PARAM(oatpp::String, content),
        PARAM(oatpp::Int32, order));

  QUERY(clearSession,
        "DELETE messages WHERE session_row_id IN"
        "(SELECT session_row_id FROM sessions "
        "WHERE user_id=:user_id AND session_id=:session_id);",
        PARAM(oatpp::Int32, user_id), PARAM(oatpp::Int32, session_id));
};

#include OATPP_CODEGEN_END(DbClient)
} // namespace Lunacd::OaRelay
