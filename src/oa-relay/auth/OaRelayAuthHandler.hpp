#pragma once

#include <auth/OaRelayJwt.hpp>

#include <cstdlib>

#include <memory>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

namespace Lunacd::OaRelay {
class AuthHandler
    : public oatpp::web::server::handler::BearerAuthorizationHandler {
public:
  AuthHandler(OATPP_COMPONENT(std::shared_ptr<Jwt>, jwt)) : m_jwt(jwt) {}

  std::shared_ptr<AuthorizationObject>
  authorize(const oatpp::String &token) override;

private:
  std::shared_ptr<Jwt> m_jwt;
};
} // namespace Lunacd::OaRelay
