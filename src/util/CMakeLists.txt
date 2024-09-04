add_library(util
    StrUtil.cpp
    SysUtil.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    StrUtil.hpp
    SysUtil.hpp
)

target_include_directories(util PRIVATE .)