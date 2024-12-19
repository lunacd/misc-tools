#include <auth/OaRelayJwt.hpp>
#include <jwt-cpp/jwt.h>

namespace Lunacd::OaRelay {
Jwt::Jwt(const oatpp::String &secret)
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
Jwt::readAndVerifyToken(const oatpp::String &token) {
  auto decoded = jwt::decode<traits>(token);
  m_verifier.verify(decoded);

  auto payload = std::make_shared<Payload>();
  payload->userId = decoded.get_payload_json().at("userId").get<int>();

  return payload;
}
} // namespace Lunacd::OaRelay
