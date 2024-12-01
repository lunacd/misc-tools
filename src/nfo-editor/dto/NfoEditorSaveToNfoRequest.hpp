#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace NfoEditor {
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class NfoEditorSaveToNfoRequest : public oatpp::DTO {

  DTO_INIT(NfoEditorSaveToNfoRequest, DTO)

  DTO_FIELD(String, filename);
  DTO_FIELD(String, title);
  DTO_FIELD(String, studio);
  DTO_FIELD(Vector<String>, actors);
  DTO_FIELD(Vector<String>, tags);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace NfoEditor