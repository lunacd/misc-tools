add_library(util
    UtilStr.cpp
    UtilSys.cpp
    UtilOat.cpp
    UtilFilesystem.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    UtilStr.hpp
    UtilSys.hpp
    UtilOat.hpp
    UtilFilesystem.hpp
)

target_include_directories(util PRIVATE .)

find_package(oatpp REQUIRED)
target_link_libraries(util PUBLIC oatpp::oatpp)
