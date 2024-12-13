#pragma once

#include <dto/OaRelayCompletionsRequest.hpp>

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/client/ApiClient.hpp>

namespace Lunacd::OaRelay {

#include OATPP_CODEGEN_BEGIN(ApiClient)
class ApiClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(ApiClient)

  API_CALL("POST", "/v1/chat/completions", getCompletions,
           HEADER(String, authorization, "Authorization"),
           BODY_DTO(Object<CompletionsRequest>, body))
};
#include OATPP_CODEGEN_END(ApiClient)

} // namespace Lunacd::OaRelay
