add_executable(oa-relay
    OaRelay.cpp
    OaRelayController.cpp
    OaRelayApiClient.cpp
    OaRelayAppComponents.cpp
    OaRelaySseScanner.cpp
)

target_sources(oa-relay PRIVATE FILE_SET HEADERS FILES
    OaRelayController.hpp
    OaRelayApiClient.hpp
    OaRelayAppComponents.hpp
    OaRelaySseScanner.hpp
    dto/OaRelayCompletionsMessage.hpp
    dto/OaRelayCompletionsRequest.hpp
    dto/OaRelayOaCompletionsRequest.hpp
)

find_package(oatpp REQUIRED)
target_link_libraries(oa-relay PRIVATE oatpp::oatpp)

find_package(oatpp-libressl REQUIRED)
target_link_libraries(oa-relay PRIVATE oatpp::oatpp-libressl)

find_package(Boost REQUIRED)
target_include_directories(oa-relay PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(oa-relay PRIVATE util)
