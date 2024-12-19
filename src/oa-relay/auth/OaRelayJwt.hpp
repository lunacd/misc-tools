#pragma once

#include <nlohmann/json.hpp>

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include <oatpp/core/Types.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

namespace Lunacd::OaRelay {
class Jwt {
  using traits = jwt::traits::nlohmann_json;

public:
  struct Payload : public oatpp::web::server::handler::AuthorizationObject {
    oatpp::Int32 userId;
  };

private:
  oatpp::String m_secret;
  jwt::verifier<jwt::default_clock, traits> m_verifier;

public:
  Jwt(const oatpp::String &secret);

  oatpp::String createToken(const std::shared_ptr<Payload> &payload);

  std::shared_ptr<Payload> readAndVerifyToken(const oatpp::String &token);
};
} // namespace Lunacd::OaRelay
