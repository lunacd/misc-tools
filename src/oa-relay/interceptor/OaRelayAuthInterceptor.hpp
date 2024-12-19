#pragma once

#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>

namespace Lunacd::OaRelay {
class AuthInterceptor
    : public oatpp::web::server::interceptor::RequestInterceptor {
public:
  AuthInterceptor();

  std::shared_ptr<OutgoingResponse>
  intercept(const std::shared_ptr<IncomingRequest> &request) override;

private:
  oatpp::web::server::handler::BearerAuthorizationHandler m_authHandler;
  oatpp::web::server::HttpRouterTemplate<bool> authEndpoints;
};
} // namespace Lunacd::OaRelay
