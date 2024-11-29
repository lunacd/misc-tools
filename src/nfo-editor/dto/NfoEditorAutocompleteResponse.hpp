#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace NfoEditor {
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class NfoEditorAutocompleteResponse : public oatpp::DTO {

  DTO_INIT(NfoEditorAutocompleteResponse, DTO /* Extends */)

  DTO_FIELD(Vector<String>, completions); // Message field
};

#include OATPP_CODEGEN_END(DTO)
} // namespace NfoEditor