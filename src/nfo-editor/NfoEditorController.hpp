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

namespace NfoEditor {
#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController {
public:
  Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ENDPOINT("GET", "/nfoEditor/complete", complete, QUERY(String, source),
           QUERY(String, str)) {
    auto completer = m_autocomplete.getCompleter(source);
    auto completions = completer.complete(str);
    auto dto = NfoEditorAutocompleteResponse::createShared();
    dto->completions = oatpp::Vector<oatpp::String>::createShared();
    for (auto &completion : completions) {
      dto->completions->emplace_back(completion);
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("POST", "/nfoEditor/saveToNfo", saveToXml,
           BODY_DTO(Object<NfoEditorSaveToNfoRequest>, dto)) {
    // Save autocompletion data
    auto completer = m_autocomplete.getCompleter("studio");
    completer.addCandidate(dto->studio);
    completer = m_autocomplete.getCompleter("actors");
    for (const auto &actor : *dto->actors) {
      completer.addCandidate(actor);
    }
    completer = m_autocomplete.getCompleter("tags");
    for (const auto &tag : *dto->tags) {
      completer.addCandidate(tag);
    }

    // Save nfo to file
    const Xml data{
        dto->title, dto->studio,
        Util::Oat::oatppVectorToStdVector<std::string, oatpp::String>(
            dto->actors),
        Util::Oat::oatppVectorToStdVector<std::string, oatpp::String>(
            dto->tags)};
    data.saveToFile(std::filesystem::path(dto->filePath));
    return createResponse(Status::CODE_200, "");
  }

private:
  Autocomplete m_autocomplete;
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace NfoEditor