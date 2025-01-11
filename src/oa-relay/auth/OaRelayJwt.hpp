#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include <nlohmann/json.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

namespace Lunacd::OaRelay {
class Jwt {
  using traits = jwt::traits::nlohmann_json;

public:
  struct Payload : public oatpp::web::server::handler::AuthorizationObject {
    oatpp::Int32 userId;
  };

  Jwt(const std::string &secret);
  oatpp::String createToken(const std::shared_ptr<Payload> &payload);
  std::shared_ptr<Payload> readAndVerifyToken(const std::string &token);

private:
  std::string m_secret;
  jwt::verifier<jwt::default_clock, traits> m_verifier;
};
} // namespace Lunacd::OaRelay
