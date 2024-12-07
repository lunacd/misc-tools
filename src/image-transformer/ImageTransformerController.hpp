#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace Lunacd::ImageTransformer {
#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController {
public:
  Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::ImageTransformer
