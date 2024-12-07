add_library(util
    UtilStr.cpp
    UtilSys.cpp
    UtilOat.cpp
    UtilFilesystem.cpp
    UtilExpiringResource.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    UtilStr.hpp
    UtilSys.hpp
    UtilOat.hpp
    UtilFilesystem.hpp
    UtilExpiringResource.hpp
)

target_include_directories(util PRIVATE .)

find_package(oatpp REQUIRED)
target_link_libraries(util PUBLIC oatpp::oatpp)

find_package(fmt REQUIRED)
target_link_libraries(util PRIVATE fmt::fmt)
