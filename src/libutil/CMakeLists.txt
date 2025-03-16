add_library(util
    UtilStr.cpp
    UtilFilesystem.cpp
    UtilSys.cpp
)

target_sources(util PUBLIC FILE_SET HEADERS FILES
    UtilStr.hpp
    UtilFilesystem.hpp
    UtilSys.hpp
)

target_include_directories(util PRIVATE .)

find_package(fmt REQUIRED)
target_link_libraries(util PRIVATE fmt::fmt)
target_include_directories(util PRIVATE ${fmt_INCLUDE_DIRS})
