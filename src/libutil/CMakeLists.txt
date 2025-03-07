add_library(util
    UtilStr.cpp
    UtilOat.cpp
    UtilFilesystem.cpp
    UtilExpiringResource.cpp
    UtilSys.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    UtilStr.hpp
    UtilOat.hpp
    UtilFilesystem.hpp
    UtilExpiringResource.hpp
    UtilSys.hpp
)

target_include_directories(util PRIVATE .)

find_package(oatpp REQUIRED)
target_link_libraries(util PUBLIC oatpp::oatpp)
target_include_directories(util PUBLIC ${oatpp_INCLUDE_DIRS})

find_package(fmt REQUIRED)
target_link_libraries(util PRIVATE fmt::fmt)
target_include_directories(util PRIVATE ${fmt_INCLUDE_DIRS})
