#pragma once

#include <OaRelayApiClient.hpp>
#include <OaRelayDTOs.hpp>
#include <OaRelayDatabaseClient.hpp>
#include <OaRelaySseScanner.hpp>
#include <UtilExpiringResource.hpp>
#include <UtilOat.hpp>
#include <auth/OaRelayJwt.hpp>

#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <stdexcept>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <jwt-cpp/jwt.h>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/stream/Stream.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/client/RequestExecutor.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/web/protocol/http/incoming/Response.hpp>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <sodium/crypto_pwhash.h>

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

class ReadCoroutine : public oatpp::async::Coroutine<ReadCoroutine> {
public:
  ReadCoroutine(
      std::shared_ptr<oatpp::web::protocol::http::incoming::Response> response,
      std::shared_ptr<SseScanner> sseScanner)
      : m_response(std::move(response)), m_sseScanner(std::move(sseScanner)) {}

  Action act() override {
    return m_response
        ->transferBodyAsync(std::make_shared<WriteCallback>(m_sseScanner))
        .next(finish());
  }

private:
  std::shared_ptr<oatpp::web::protocol::http::incoming::Response> m_response;
  std::shared_ptr<SseScanner> m_sseScanner;
};

#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController {
public:
  typedef Controller __ControllerType;
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

  ENDPOINT_ASYNC("POST", "/oaRelay/signUp", SignUpAsync) {
    ENDPOINT_ASYNC_INIT(SignUpAsync);
    OATPP_COMPONENT(std::shared_ptr<Jwt>, m_jwt);
    OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, m_databaseClient);

    Action act() override {
      return request
          ->readBodyToDtoAsync<oatpp::Object<SignInRequest>>(
              controller->getDefaultObjectMapper())
          .callbackTo(&SignUpAsync::onBodyObtained);
    }

    Action onBodyObtained(const oatpp::Object<SignInRequest> &dto) {
      // Create password hash
      char hash[crypto_pwhash_STRBYTES];
      if (crypto_pwhash_str(hash, dto->password->c_str(), dto->password->size(),
                            crypto_pwhash_OPSLIMIT_INTERACTIVE,
                            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        return _return(controller->createResponse(Status::CODE_500,
                                                  "failed to create user"));
      }

      // Save user to database
      const auto queryResult =
          m_databaseClient->createUser(dto->username, std::string(hash));
      if (!queryResult->isSuccess()) {
        return _return(controller->createResponse(
            Status::CODE_500, queryResult->getErrorMessage()));
      }
      return _return(controller->createResponse(Status::CODE_200, "success"));
    }
  };

  ENDPOINT_ASYNC("POST", "/oaRelay/signIn", SignInAsync) {
    ENDPOINT_ASYNC_INIT(SignInAsync);
    OATPP_COMPONENT(std::shared_ptr<Jwt>, m_jwt);
    OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, m_databaseClient);

    Action act() override {
      return request
          ->readBodyToDtoAsync<oatpp::Object<SignInRequest>>(
              controller->getDefaultObjectMapper())
          .callbackTo(&SignInAsync::onBodyObtained);
    }

    Action onBodyObtained(const oatpp::Object<SignInRequest> &dto) {
      // Get password hash from database
      const auto queryResult = m_databaseClient->getUserPassword(dto->username);
      if (!queryResult->isSuccess()) {
        return _return(controller->createResponse(
            Status::CODE_500, queryResult->getErrorMessage()));
      }
      const auto users =
          queryResult->fetch<oatpp::Vector<oatpp::Object<DbUser>>>();
      if (users->size() == 0) {
        return _return(
            controller->createResponse(Status::CODE_401, "password incorrect"));
      }
      const auto dbUser = users[0];

      // Verify password
      if (crypto_pwhash_str_verify(dbUser->password->c_str(),
                                   dto->password->c_str(),
                                   dto->password->size()) != 0) {
        return _return(
            controller->createResponse(Status::CODE_401, "password incorrect"));
      }

      // Password is correct, create and return token
      const auto payload = std::make_shared<Jwt::Payload>();
      payload->userId = dbUser->user_id;
      const auto token = m_jwt->createToken(payload);
      auto response = controller->createResponse(Status::CODE_200, "");
      // Set cookie to expire in 1 week
      response->putHeader(
          "Set-Cookie",
          std::format(
              "token={}; SameSite=Strict; Secure; HttpOnly; Max-Age=604800",
              token->c_str()));

      return _return(response);
    }
  };

  ENDPOINT_ASYNC("POST", "/oaRelay/newChat", NewChatAsync) {
    ENDPOINT_ASYNC_INIT(NewChatAsync);
    OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, m_databaseClient);

    Action act() override {
      const auto userId = request->getBundleData<oatpp::Int32>("userId");
      m_databaseClient->clearSession(userId, 1);
      m_databaseClient->newMessage(userId, 1, "system",
                                   "You are a helpful assistant.", 0);
      return _return(controller->createResponse(Status::CODE_200, ""));
    }
  };

  ENDPOINT_ASYNC("POST", "/oaRelay/completions", CompletionsAsync) {
    ENDPOINT_ASYNC_INIT(CompletionsAsync);
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, m_executor);
    OATPP_COMPONENT(std::shared_ptr<DatabaseClient>, m_databaseClient);

    Action act() override {
      return request
          ->readBodyToDtoAsync<oatpp::Object<CompletionsRequest>>(
              controller->getDefaultObjectMapper())
          .callbackTo(&CompletionsAsync::onBodyObtained);
    }

    Action onBodyObtained(const oatpp::Object<CompletionsRequest> &dto) {
      // Get past messages
      // This currently only support 1 session
      auto queryResult = m_databaseClient->getMessages(1);
      auto messages =
          queryResult
              ->fetch<oatpp::Vector<oatpp::Object<CompletionsMessage>>>();
      const auto newMessage = CompletionsMessage::createShared();
      newMessage->role = "user";
      newMessage->content = dto->message;
      messages->emplace_back(newMessage);

      // Save new message
      m_databaseClient->newMessage(1, 1, "user", dto->message,
                                   messages->size() - 1);

      // Request to OpenAI API
      const auto oaDto = OaCompletionsRequest::createShared();
      oaDto->model = "gpt-4o-mini";
      oaDto->messages = messages;
      oaDto->stream = true;
      return controller->m_apiClient
          ->getCompletionsAsync(controller->m_apiKey, oaDto)
          .callbackTo(&CompletionsAsync::onResponseObtained);
    }

    Action onResponseObtained(const std::shared_ptr<IncomingResponse> &apiRes) {
      const auto responseType = apiRes->getHeader(Header::CONTENT_TYPE);

      // Forward response
      if (!responseType->starts_with("text/event-stream")) {
        // Not a stream, then just forward the response
        const auto body = apiRes->readBodyToString();
        const auto statusCode = apiRes->getStatusCode();
        const auto statusDescription = apiRes->getStatusDescription();
        const auto status = oatpp::web::protocol::http::Status{
            statusCode, statusDescription->c_str()};
        const auto res = controller->createResponse(status, body);
        res->putHeader(Header::CONTENT_TYPE,
                       apiRes->getHeader(Header::CONTENT_TYPE));
        return _return(res);
      } else {
        // This is an event stream, stream it
        const auto sseScanner = std::make_shared<SseScanner>();
        m_executor->execute<ReadCoroutine>(apiRes, sseScanner);

        const auto body = std::make_shared<
            oatpp::web::protocol::http::outgoing::StreamingBody>(
            std::make_shared<ReadCallback>(sseScanner));
        const auto res =
            std::make_shared<OutgoingResponse>(Status::CODE_200, body);
        res->putHeader("Cache-Control", "no-store");
        res->putHeader("Content-Type", "text/event-stream");
        return _return(res);
      }
    }
  };

private:
  std::shared_ptr<ApiClient> m_apiClient;
  std::string m_apiKey;

  friend class CompletionsAsync;
};

#include OATPP_CODEGEN_END(ApiController)
} // namespace Lunacd::OaRelay
