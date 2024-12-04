#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class NfoEditorSaveToNfoResponse : public oatpp::DTO {

  DTO_INIT(NfoEditorSaveToNfoResponse, DTO)

  DTO_FIELD(Int32, id);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::NfoEditor