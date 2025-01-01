#pragma once

#include "UtilStr.hpp"
#include <NfoEditorAutocomplete.hpp>
#include <NfoEditorXml.hpp>
#include <UtilExpiringResource.hpp>
#include <UtilOat.hpp>
#include <dto/NfoEditorAutocompleteResponse.hpp>
#include <dto/NfoEditorSaveToNfoRequest.hpp>
#include <dto/NfoEditorSaveToNfoResponse.hpp>

#include <cmath>
#include <cstdlib>
#include <mutex>
#include <string>
#include <unordered_map>

#include <boost/url.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <fmt/format.h>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace Lunacd::NfoEditor {
#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController {
public:
  Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        m_xmlCacheExpiring{
            600, [this](const std::string &key) { m_xmlCache.erase(key); }} {}

  ENDPOINT("GET", "/nfoEditor/complete", complete, QUERY(String, source),
           QUERY(String, str)) {
    const boost::urls::decode_view decodedStr(std::string{str});
    std::lock_guard<std::mutex> lock{m_autocompleteLock};

    auto completer = m_autocomplete.getCompleter(source);
    auto completions = completer.complete(
        Util::Str::toLower(std::string{decodedStr.begin(), decodedStr.end()}));
    auto dto = NfoEditorAutocompleteResponse::createShared();
    dto->completions = oatpp::Vector<oatpp::String>::createShared();
    for (auto &completion : completions) {
      dto->completions->emplace_back(completion);
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("POST", "/nfoEditor/saveToNfo", saveToNfo,
           BODY_DTO(Object<NfoEditorSaveToNfoRequest>, dto)) {
    std::lock_guard<std::mutex> lock{m_autocompleteLock};

    // Save autocompletion data
    auto completer = m_autocomplete.getCompleter("studio");
    completer.addCandidate(dto->studio);
    completer = m_autocomplete.getCompleter("actor");
    for (const auto &actor : *dto->actors) {
      completer.addCandidate(actor);
    }
    completer = m_autocomplete.getCompleter("tag");
    for (const auto &tag : *dto->tags) {
      completer.addCandidate(tag);
    }
    m_autocomplete.exportCompletionData();

    // Save nfo to file
    std::lock_guard<std::mutex> xmlLock{m_xmlCacheLock};
    const Xml data{
        dto->filename, dto->title, dto->studio,
        Util::Oat::oatppVectorToStdVector<std::string, oatpp::String>(
            dto->actors),
        Util::Oat::oatppVectorToStdVector<std::string, oatpp::String>(
            dto->tags)};

    std::string uuid;
    {
      std::lock_guard<std::mutex> uuidLock{m_uuidGeneratorLock};
      uuid = to_string(m_uuidGenerator());
    }
    m_xmlCache.emplace(uuid, data);
    m_xmlCacheExpiring.insert(uuid);

    auto dtoResponse = NfoEditorSaveToNfoResponse::createShared();
    dtoResponse->uuid = uuid;

    return createDtoResponse(Status::CODE_200, dtoResponse);
  }

  ENDPOINT("GET", "/nfoEditor/getNfo", getNfo, QUERY(String, uuid)) {
    std::lock_guard<std::mutex> lock{m_xmlCacheLock};
    const auto it = m_xmlCache.find(uuid);
    if (it == m_xmlCache.end()) {
      return createResponse(Status::CODE_404,
                            "No data associated with the given ID");
    }
    std::string content = it->second.exportToStr();
    auto response = createResponse(Status::CODE_200, content);
    response->putHeader(
        "Content-Disposition",
        fmt::format("attachment; filename=\"{}\"", it->second.filename));
    return response;
  }

private:
  Autocomplete m_autocomplete;
  std::mutex m_autocompleteLock;

  Util::ExpiringResource::ExpiringResource<std::string> m_xmlCacheExpiring;
  std::unordered_map<std::string, Xml> m_xmlCache;
  std::mutex m_xmlCacheLock;

  boost::uuids::random_generator m_uuidGenerator;
  std::mutex m_uuidGeneratorLock;
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::NfoEditor