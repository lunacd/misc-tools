add_library(util
    PathUtil.cpp
    StrUtil.cpp
    SysUtil.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    PathUtil.hpp
    StrUtil.hpp
    SysUtil.hpp
)

target_include_directories(util PRIVATE .)