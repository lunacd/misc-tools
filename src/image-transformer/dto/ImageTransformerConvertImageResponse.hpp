#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace Lunacd::ImageTransformer {
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class ImageTransformerConvertImageResponse : public oatpp::DTO {

  DTO_INIT(ImageTransformerConvertImageResponse, DTO /* Extends */)

  DTO_FIELD(String, uuid); // Message field
};

#include OATPP_CODEGEN_END(DTO)
} // namespace Lunacd::ImageTransformer
