#include <auth/OaRelayAuthHandler.hpp>

namespace Lunacd::OaRelay {
std::shared_ptr<oatpp::web::server::handler::AuthorizationObject>
AuthHandler::authorize(const oatpp::String &token) {
  const auto payload = m_jwt->readAndVerifyToken(token);
  return payload;
}
} // namespace Lunacd::OaRelay
