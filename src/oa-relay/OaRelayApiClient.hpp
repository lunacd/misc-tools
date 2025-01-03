#pragma once

#include <OaRelayDTOs.hpp>

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/client/ApiClient.hpp>

namespace Lunacd::OaRelay {

#include OATPP_CODEGEN_BEGIN(ApiClient)
class ApiClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(ApiClient)

  API_CALL_ASYNC("POST", "/v1/chat/completions", getCompletionsAsync,
                 HEADER(String, authorization, "Authorization"),
                 BODY_DTO(Object<OaCompletionsRequest>, body))
};
#include OATPP_CODEGEN_END(ApiClient)

} // namespace Lunacd::OaRelay
