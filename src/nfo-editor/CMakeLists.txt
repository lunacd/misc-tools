add_executable(nfo-editor)

target_sources(nfo-editor PRIVATE
    NfoEditor.cpp
    NfoEditorXml.cpp
    NfoEditorController.cpp
    NfoEditorAutocomplete.cpp
    NfoEditorAppComponents.cpp
    NfoEditorDatabaseClient.cpp
    NfoEditorDtos.cpp
    NfoEditorScrapeAutocomplete.cpp
)

target_sources(nfo-editor PRIVATE FILE_SET HEADERS FILES
    NfoEditorXml.hpp
    NfoEditorController.hpp
    NfoEditorAutocomplete.hpp
    NfoEditorAppComponents.hpp
    NfoEditorDatabaseClient.hpp
    NfoEditorDtos.hpp
    NfoEditorScrapeAutocomplete.hpp
)

find_package(fmt REQUIRED)
target_link_libraries(nfo-editor PRIVATE fmt::fmt)

find_package(pugixml REQUIRED)
target_link_libraries(nfo-editor PRIVATE pugixml::pugixml)

find_package(marisa REQUIRED)
target_link_libraries(nfo-editor PRIVATE marisa::marisa)

find_package(oatpp REQUIRED)
target_link_libraries(nfo-editor PRIVATE oatpp::oatpp)

find_package(oatpp-postgresql REQUIRED)
target_link_libraries(nfo-editor PRIVATE oatpp::oatpp-postgresql)

find_package(Boost REQUIRED)
target_include_directories(nfo-editor PRIVATE ${Boost_INCLUDE_DIRS})

find_package(CURL REQUIRED)
target_link_libraries(nfo-editor PRIVATE CURL::libcurl)

target_link_libraries(nfo-editor PRIVATE util)
