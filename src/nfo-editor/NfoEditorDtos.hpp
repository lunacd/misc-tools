#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(DTO)

class NfoEditorAutocompleteResponse : public oatpp::DTO {

  DTO_INIT(NfoEditorAutocompleteResponse, DTO /* Extends */)

  DTO_FIELD(Vector<String>, completions); // Message field
};

class NfoEditorSaveToNfoRequest : public oatpp::DTO {

  DTO_INIT(NfoEditorSaveToNfoRequest, DTO)

  DTO_FIELD(String, filename);
  DTO_FIELD(String, title);
  DTO_FIELD(String, studio);
  DTO_FIELD(Vector<String>, actors);
  DTO_FIELD(Vector<String>, tags);
};

class NfoEditorSaveToNfoResponse : public oatpp::DTO {

  DTO_INIT(NfoEditorSaveToNfoResponse, DTO)

  DTO_FIELD(String, uuid);
};

class DbCompletionRow : public oatpp::DTO {
  DTO_INIT(DbCompletionRow, DTO)

  DTO_FIELD(String, content);
  DTO_FIELD(String, source);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::NfoEditor
