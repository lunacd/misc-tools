add_library(util
    UtilStr.cpp
    UtilSys.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    UtilStr.hpp
    UtilSys.hpp
)

target_include_directories(util PRIVATE .)
