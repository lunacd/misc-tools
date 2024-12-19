#include <auth/OaRelayJwt.hpp>
#include <interceptor/OaRelayAuthInterceptor.hpp>

namespace Lunacd::OaRelay {
AuthInterceptor::AuthInterceptor() {
  authEndpoints.route("POST", "users/signup", false);
  authEndpoints.route("POST", "users/signin", false);

  authEndpoints.route("GET", "swagger/*", false);
  authEndpoints.route("GET", "api-docs/oas-3.0.0.json", false);
}

std::shared_ptr<AuthInterceptor::OutgoingResponse>
AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest> &request) {

  auto r = authEndpoints.getRoute(request->getStartingLine().method,
                                  request->getStartingLine().path);
  if (r && !r.getEndpoint()) {
    return nullptr; // Continue without Authorization
  }

  auto authHeader =
      request->getHeader(oatpp::web::protocol::http::Header::AUTHORIZATION);

  auto authObject = std::static_pointer_cast<Jwt::Payload>(
      m_authHandler.handleAuthorization(authHeader));
  if (authObject) {
    request->putBundleData("userId", authObject->userId);
    return nullptr; // Continue - token is valid.
  }

  throw oatpp::web::protocol::http::HttpError(
      oatpp::web::protocol::http::Status::CODE_401, "Unauthorized", {});
}
} // namespace Lunacd::OaRelay
