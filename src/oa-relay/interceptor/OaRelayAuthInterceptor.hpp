#pragma once

#include <auth/OaRelayJwt.hpp>

#include <memory>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp/core/macro/component.hpp>

namespace Lunacd::OaRelay {
class AuthInterceptor
    : public oatpp::web::server::interceptor::RequestInterceptor {
public:
  AuthInterceptor(OATPP_COMPONENT(std::shared_ptr<Jwt>, jwt)) : m_jwt(jwt) {
    authEndpoints.route("POST", "/oaRelay/signIn", false);
    authEndpoints.route("POST", "/oaRelay/signUp", false);
  }

  std::shared_ptr<OutgoingResponse>
  intercept(const std::shared_ptr<IncomingRequest> &request) override;

private:
  std::shared_ptr<Jwt> m_jwt;
  oatpp::web::server::HttpRouterTemplate<bool> authEndpoints;
};
} // namespace Lunacd::OaRelay
