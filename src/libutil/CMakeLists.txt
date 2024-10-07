add_library(util)

target_sources(util PUBLIC FILE_SET CXX_MODULES FILES
    Util.cppm
    UtilSys.cppm
    UtilStr.cppm
)

target_include_directories(util PRIVATE .)