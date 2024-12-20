#pragma once

#include <dto/OaRelayCompletionsMessage.hpp>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::OaRelay {
#include OATPP_CODEGEN_BEGIN(DTO)

class CompletionsRequest : public oatpp::DTO {
  DTO_INIT(CompletionsRequest, DTO);

  DTO_FIELD(Vector<Object<CompletionsMessage>>, messages);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::OaRelay
