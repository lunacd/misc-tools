#pragma once

#include <NfoEditorAutocomplete.hpp>
#include <NfoEditorDtos.hpp>
#include <NfoEditorXml.hpp>
#include <UtilExpiringResource.hpp>
#include <UtilOat.hpp>
#include <UtilStr.hpp>

#include <cmath>
#include <cstdlib>
#include <mutex>
#include <string>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <curl/curl.h>
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
            600, [this](const std::string &key) { m_xmlCache.erase(key); }} {
    m_curl = curl_easy_init();
  }

  ~Controller() { curl_easy_cleanup(m_curl); }

  ENDPOINT("GET", "/nfoEditor/complete", complete, QUERY(String, source),
           QUERY(String, str)) {
    int decodedLength;
    char *decodedStr =
        curl_easy_unescape(m_curl, str->c_str(), str->size(), &decodedLength);
    std::lock_guard<std::mutex> lock{m_autocompleteLock};
    auto completer = m_autocomplete.getCompleter(source);
    auto completions = completer.complete(Util::Str::toLower(decodedStr));
    curl_free(decodedStr);
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
    m_autocomplete.addCompletionCandidate("studio", dto->studio);
    completer = m_autocomplete.getCompleter("actor");
    for (const auto &actor : *dto->actors) {
      m_autocomplete.addCompletionCandidate("actor", actor);
    }
    completer = m_autocomplete.getCompleter("tag");
    for (const auto &tag : *dto->tags) {
      m_autocomplete.addCompletionCandidate("tag", tag);
    }

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
  CURL *m_curl;
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