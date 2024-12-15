#pragma once

#include <OaRelayApiClient.hpp>
#include <OaRelaySseScanner.hpp>
#include <UtilExpiringResource.hpp>
#include <UtilOat.hpp>
#include <dto/OaRelayCompletionsMessage.hpp>
#include <dto/OaRelayCompletionsRequest.hpp>
#include <dto/OaRelayOaCompletionsRequest.hpp>

#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <oatpp/web/protocol/http/Http.hpp>
#include <stdexcept>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/stream/Stream.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/client/RequestExecutor.hpp>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace Lunacd::OaRelay {

class WriteCallback : public oatpp::data::stream::WriteCallback {
public:
  WriteCallback(std::shared_ptr<SseScanner> sseScanner)
      : m_sseScanner{std::move(sseScanner)} {}

  oatpp::v_io_size
  write(const void *data, v_buff_size count,
        [[maybe_unused]] oatpp::async::Action &action) override {
    const char *textBuffer = reinterpret_cast<const char *>(data);
    m_sseScanner->addInput(textBuffer, count);
    return count;
  }

private:
  std::shared_ptr<SseScanner> m_sseScanner;
};

class ReadCallback : public oatpp::data::stream::ReadCallback {
public:
  ReadCallback(std::shared_ptr<SseScanner> sseScanner)
      : m_sseScanner{std::move(sseScanner)} {};

  oatpp::v_io_size read(void *buffer, v_buff_size count,
                        oatpp::async::Action &action) override {
    // Send data remaining in buffer
    if (!m_remaining.empty()) {
      size_t bytesRead =
          std::min(static_cast<size_t>(count), m_remaining.size());
      m_remaining.copy(static_cast<char *>(buffer), bytesRead);
      std::cout << "Sending " << m_remaining.substr(0, bytesRead);
      m_remaining.erase(0, bytesRead);
      return bytesRead;
    }

    // If buffer is empty and this stream is finished, stop streaming
    if (m_finished) {
      return 0;
    }

    // Wait for new event
    std::unique_lock<std::mutex> lock{m_sseScanner->mutex};
    m_sseScanner->cv.wait(lock, [this]() { return m_sseScanner->hasEvent(); });
    std::string event = m_sseScanner->getEvent();

    // Check stop condition
    if (event == "data: [DONE]") {
      m_finished = true;
    }
    event += "\n\n";

    size_t bytesRead = std::min(static_cast<size_t>(count), event.size());
    event.copy(static_cast<char *>(buffer), bytesRead);
    if (bytesRead < event.size()) {
      m_remaining = event.substr(bytesRead);
    }

    return bytesRead;
  }

private:
  bool m_finished = false;
  std::string m_remaining;
  std::shared_ptr<SseScanner> m_sseScanner;
};

#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController {
public:
  Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
             OATPP_COMPONENT(std::shared_ptr<ApiClient>, apiClient))
      : oatpp::web::server::api::ApiController(objectMapper),
        m_apiClient{apiClient} {
    const auto key = std::getenv("OPENAI_API_KEY");
    if (key == nullptr) {
      throw std::runtime_error("OpenAI key should be provided via "
                               "OPENAI_API_KEY environment variable");
    }
    m_apiKey = std::format("Bearer {}", key);
  }

  ENDPOINT("POST", "/oaRelay/completions", completions,
           BODY_DTO(Object<CompletionsRequest>, dto)) {
    // Request to OpenAI API
    const auto oaDto = OaCompletionsRequest::createShared();
    oaDto->model = "gpt-4o-mini";
    oaDto->messages = dto->messages;
    oaDto->stream = true;
    const auto apiRes = m_apiClient->getCompletions(m_apiKey, oaDto);
    const auto responseType = apiRes->getHeader(Header::CONTENT_TYPE);

    // Forward response
    if (responseType != "text/event-stream") {
      // Not a stream, then just forward the response
      const auto body = apiRes->readBodyToString();
      const auto statusCode = apiRes->getStatusCode();
      const auto statusDescription = apiRes->getStatusDescription();
      const auto status = oatpp::web::protocol::http::Status{
          statusCode, statusDescription->c_str()};
      const auto res = createResponse(status, body);
      res->putHeader(Header::CONTENT_TYPE,
                     apiRes->getHeader(Header::CONTENT_TYPE));
      return res;
    } else {
      // This is an event stream, stream it
      const auto sseScanner = std::make_shared<SseScanner>();
      apiRes->transferBody(std::make_shared<WriteCallback>(sseScanner));

      const auto body =
          std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
              std::make_shared<ReadCallback>(sseScanner));
      const auto res =
          std::make_shared<OutgoingResponse>(Status::CODE_200, body);
      res->putHeader("Cache-Control", "no-store");
      res->putHeader("Content-Type", "text/event-stream");
      return res;
    }
  }

private:
  std::shared_ptr<oatpp::web::client::RequestExecutor> m_requestExecutor;
  std::shared_ptr<ApiClient> m_apiClient;
  std::string m_apiKey;
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::OaRelay
