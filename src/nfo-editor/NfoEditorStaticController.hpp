#pragma once

#include <NfoEditorAutocomplete.hpp>
#include <NfoEditorXml.hpp>
#include <UtilOat.hpp>
#include <cmath>
#include <dto/NfoEditorAutocompleteResponse.hpp>
#include <dto/NfoEditorSaveToNfoRequest.hpp>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(ApiController)

class StaticController : public oatpp::web::server::api::ApiController {
public:
  StaticController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ADD_CORS(staticFiles, "*")
  ENDPOINT("GET", "/ui/*", staticFiles,
           REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    const std::string tail = request->getPathTail();
    

    return createResponse(Status::CODE_200, tail);
  }
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace NfoEditor