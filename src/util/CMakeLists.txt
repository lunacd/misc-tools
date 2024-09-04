add_library(util)

target_sources(util PUBLIC FILE_SET CXX_MODULES FILES
    Util.cppm
    SysUtil.cppm
    StrUtil.cppm
)

target_include_directories(util PRIVATE .)