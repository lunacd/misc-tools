add_executable(oa-relay
    OaRelay.cpp
    OaRelayController.cpp
    OaRelayApiClient.cpp
    OaRelayAppComponents.cpp
    OaRelaySseScanner.cpp
    interceptor/OaRelayAuthInterceptor.cpp
    auth/OaRelayJwt.cpp
    auth/OaRelayAuthHandler.cpp
)

target_sources(oa-relay PRIVATE FILE_SET HEADERS FILES
    OaRelayController.hpp
    OaRelayApiClient.hpp
    OaRelayAppComponents.hpp
    OaRelaySseScanner.hpp
    OaRelayDTOs.hpp
    interceptor/OaRelayAuthInterceptor.hpp
    auth/OaRelayJwt.hpp
    auth/OaRelayAuthHandler.hpp
)

find_package(oatpp REQUIRED)
target_link_libraries(oa-relay PRIVATE oatpp::oatpp)

find_package(oatpp-openssl REQUIRED)
target_link_libraries(oa-relay PRIVATE oatpp::oatpp-openssl)

find_package(oatpp-postgresql REQUIRED)
target_link_libraries(oa-relay PRIVATE oatpp::oatpp-postgresql)

find_package(jwt-cpp REQUIRED)
target_link_libraries(oa-relay PRIVATE jwt-cpp::jwt-cpp)

find_package(nlohmann_json REQUIRED)
target_link_libraries(oa-relay PRIVATE nlohmann_json::nlohmann_json)

find_package(libsodium REQUIRED)
target_link_libraries(oa-relay PRIVATE libsodium::libsodium)

find_package(Boost REQUIRED)
target_include_directories(oa-relay PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(oa-relay PRIVATE util)
