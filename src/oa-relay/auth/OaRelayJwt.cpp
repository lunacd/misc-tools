#include <auth/OaRelayJwt.hpp>
#include <jwt-cpp/jwt.h>

namespace Lunacd::OaRelay {
Jwt::Jwt(const std::string &secret)
    : m_secret(secret), m_verifier(jwt::verify<traits>().allow_algorithm(
                            jwt::algorithm::hs256{secret})) {}

oatpp::String Jwt::createToken(const std::shared_ptr<Payload> &payload) {
  auto token =
      jwt::create<traits>()
          .set_type("JWS")
          .set_payload_claim("userId",
                             jwt::basic_claim<traits>(*payload->userId.get()))
          .sign(jwt::algorithm::hs256{m_secret});
  return token;
}

std::shared_ptr<Jwt::Payload>
Jwt::readAndVerifyToken(const std::string &token) {
  auto decoded = jwt::decode<traits>(token);
  try {
    m_verifier.verify(decoded);
  } catch (jwt::error::token_verification_exception &ex) {
    return nullptr;
  }

  auto payload = std::make_shared<Payload>();
  payload->userId = decoded.get_payload_json().at("userId").get<int>();

  return payload;
}
} // namespace Lunacd::OaRelay
