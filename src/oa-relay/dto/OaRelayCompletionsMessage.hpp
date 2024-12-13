#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::OaRelay {
#include OATPP_CODEGEN_BEGIN(DTO)

class CompletionsMessage : public oatpp::DTO {
  DTO_INIT(CompletionsMessage, DTO);

  DTO_FIELD(String, role);
  DTO_FIELD(String, content);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::OaRelay
