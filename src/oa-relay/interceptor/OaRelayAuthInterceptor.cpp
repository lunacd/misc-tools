#include <UtilOat.hpp>
#include <interceptor/OaRelayAuthInterceptor.hpp>

#include <memory>

#include <auth/OaRelayJwt.hpp>
#include <interceptor/OaRelayAuthInterceptor.hpp>
#include <oatpp/core/macro/component.hpp>

namespace Lunacd::OaRelay {

std::shared_ptr<AuthInterceptor::OutgoingResponse>
AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest> &request) {

  auto r = authEndpoints.getRoute(request->getStartingLine().method,
                                  request->getStartingLine().path);
  if (r && !r.getEndpoint()) {
    return nullptr; // Continue without Authorization
  }

  auto authHeader =
      request->getHeader(oatpp::web::protocol::http::Header::AUTHORIZATION);

  const auto token = Util::Oat::getCookie(request, "oaRelayToken");
  if (token) {
    const auto payload = m_jwt->readAndVerifyToken(*token);
    if (payload) {
      request->putBundleData("userId", payload->userId);
      return nullptr; // Continue - token is valid.
    }
  }
  return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
      oatpp::web::protocol::http::Status::CODE_401, "unauthorized");
}
} // namespace Lunacd::OaRelay
