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

class CompletionsRequest : public oatpp::DTO {
  DTO_INIT(CompletionsRequest, DTO);

  DTO_FIELD(String, message);
};

class OaCompletionsRequest : public oatpp::DTO {
  DTO_INIT(OaCompletionsRequest, DTO);

  DTO_FIELD(String, model);
  DTO_FIELD(Vector<Object<CompletionsMessage>>, messages);
  DTO_FIELD(Boolean, stream);
};

class SignInRequest : public oatpp::DTO {
  DTO_INIT(SignInRequest, DTO);

  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
};

class DbUser : public oatpp::DTO {
  DTO_INIT(DbUser, DTO);

  DTO_FIELD(Int32, user_id);
  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::OaRelay
