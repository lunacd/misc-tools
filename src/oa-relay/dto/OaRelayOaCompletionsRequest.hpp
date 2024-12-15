#pragma once

#include <dto/OaRelayCompletionsMessage.hpp>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::OaRelay {
#include OATPP_CODEGEN_BEGIN(DTO)

class OaCompletionsRequest : public oatpp::DTO {
  DTO_INIT(OaCompletionsRequest, DTO);

  DTO_FIELD(String, model);
  DTO_FIELD(Vector<Object<CompletionsMessage>>, messages);
  DTO_FIELD(Boolean, stream);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::OaRelay
